#pragma once

#include <util/shared_ptr.h>

#include <string>
#include <map>
#include <sstream>

namespace base
{
	namespace bson
	{
		BASE_DECL_SMARTPTR_STRUCT(BSON);
		BASE_DECL_SMARTPTR_STRUCT(Packet);

		// http://stackoverflow.com/questions/1448467/initializing-a-c-stdistringstream-from-an-in-memory-buffer/1449527#1449527
		struct OneShotReadBuf : public std::streambuf
		{
			OneShotReadBuf(char* s, std::size_t n)
			{
				setg(s, s, s + n);
			}
		};


		std::ostream & serialiseInt(std::ostream & out, const int n );
		std::istream & deserialiseInt(std::istream & in, int &n );

		std::ostream & serialiseString(std::ostream & out, const std::string &str );
		std::istream & deserialiseString(std::istream & in, std::string &str );

		// ===============================================
		// Key
		// ===============================================

		struct Key
		{
			Key();
			Key( const std::string &key );
			Key( const int key );

			int               hash() const;

			bool                m_isString; // if false, key is int
			std::string        m_stringKey;
			int                   m_intKey;




		};

		bool operator<( const Key &lhs, const Key &rhs );
		std::ostream & operator<<(std::ostream & out, const Key &key );
		std::istream & operator>>(std::istream & in, Key &key );

		// ===============================================
		// Item
		// ===============================================

		BASE_DECL_SMARTPTR_STRUCT(Item);
		struct Item
		{
			Item();

			template<typename T>
			void set( const T &value );
			//void set(const char* &other);

			char          *m_data;
			int            m_size;
			BSONPtr       m_child;
		};

		template<typename T>
		void Item::set( const T &item )
		{
			m_size = sizeof(T);
			m_data = (char *)realloc( m_data, m_size );
			memcpy(m_data, &item, m_size);
			if(m_child)
				m_child = BSONPtr();
		}


		template<>
		inline void Item::set<std::string>(const std::string &other)
		{
			m_size = (int)other.size();
			m_data = (char *)realloc( m_data, m_size );
			memcpy(m_data, &other[0], m_size);
			if(m_child)
				m_child = BSONPtr();
		}

		/*
		// problem: for some reason we can not specialize for const char *
		// as soon as we add a ptr it errors :(
		template<>
		inline void Item::set<int*>(const int* &other)
		{
			m_size = (int)other.size();
			m_data = (char *)realloc( m_data, m_size );
			memcpy(m_data, &other[0], m_size);
			if(m_child)
				m_child = BSONPtr();
		}
		*/

		template<>
		inline void Item::set<BSONPtr>(const BSONPtr &other)
		{
			m_size = 0;
			if( m_data )
			{
				free(m_data);
				m_data = 0;
			}
			m_child = other;
		}

		std::ostream & operator<<(std::ostream & out, const Item & );
		std::istream & operator>>(std::istream & in, Item &item );
	
		// ===============================================
		// ItemHelper
		// ===============================================

		struct ItemHelper
		{
			ItemHelper();
			ItemHelper( BSONPtr bson, const Key &key );

			ItemHelper      operator[]( const std::string &key ); // will only work if wrapped item is a bson object
			ItemHelper      operator[]( const int &key ); // will only work if wrapped item is a bson object

			// must support pods, bson objects
			template<typename T>
			ItemHelper & operator = (const T &other);

			template<typename T>
			operator T ();

			std::string asString();


			BSONPtr                           m_bson;
			Key                                m_key;
			ItemPtr                           m_item; // the item which is wrapped by the helper - invalid pointer means: item not yet created
		};

		// ItemHelper assignment operator
		template<typename T>
		ItemHelper & ItemHelper::operator = (const T &other)
		{
			if(!m_item)
			{
				m_item = ItemPtr(new Item());
				// no items means, it hasnt been added to the BSON yet
				m_bson->set( m_key, m_item );
			}
			m_item->set<T>(other);

			return *this;
		}

		template <typename T>
		ItemHelper::operator T()
		{
			T data;
			memcpy( &data, m_item->m_data, sizeof(T));
			return data;
		}



		// for some reason I didnt got template specializatino to work for strings :(
		//inline operator std::string()
		//{
		//	std::string data( (char *) m_item->m_data, m_item->m_size );
		//	return data;
		//}





		// ===============================================
		// Helper
		// ===============================================

		struct Helper
		{
			Helper(BSONPtr bson);

			bool                           contains( const Key &key );

			ItemHelper      operator[]( const std::string &key ); // select an item within the bson object
			ItemHelper      operator[]( const int &key ); // select an item within the bson object

			template<typename T>
			Helper                  &operator+=( const T &value ); // append item

			operator BSONPtr ();                                  // ptr conversion operator


			BSONPtr m_bson;
		};


		template <>
		inline ItemHelper::operator Helper()
		{
			return Helper(m_item->m_child);
		}
		template <>
		inline ItemHelper::operator BSONPtr()
		{
			return m_item->m_child;
		}



		// ===============================================
		// BSON
		// ===============================================
		struct BSON
		{
			typedef std::map<Key, ItemPtr> ItemMap;

			BSON();

			bool                           contains( const Key &key );
			ItemPtr                             get( const Key &key );
			void                  set( const Key &key, ItemPtr item );
			void                               append( ItemPtr item ); // uses a local counter
			int                                      numItems() const;

			ItemMap                                           m_items;
			int                                           m_nextIndex;

		};
		std::ostream & operator<<(std::ostream & out, const BSON & );
		std::istream & operator>>(std::istream & in, BSON & );

		Helper create();



	
		template<typename T>
		Helper & Helper::operator+=( const T &value )
		{
			ItemPtr item = ItemPtr(new Item());
			item->set<T>(value);
			m_bson->append( item );
			return *this;
		}


		/*
		// we added these specialicatinos here because otherwise we would always use Item::set<T> generic version...
		template<>
		inline Helper & Helper::operator+=<std::string>( std::string &value )
		{
			ItemPtr item = ItemPtr(new Item());
			item->set(value);
			m_bson->append( item );
			return *this;
		}
		*/

		// ===============================================
		// Item
		// ===============================================

		
		template<>
		inline void Item::set( const Helper &child )
		{
			if( m_data )
			{
				m_size = 0;
				free(m_data);
			}
			m_child = child.m_bson;
		} 

		// ===============================================
		// Packet
		// ===============================================
		struct Packet
		{
			Packet( char *data, int size );
			Packet( const std::string &data );
			Packet();
			~Packet();
			char          *m_data;
			int            m_size;
		};

		PacketPtr pack( Helper &helper );
		Helper unpack( PacketPtr &packet );
		Helper unpack( char *data, int size );
		
	}
}