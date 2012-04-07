
#include "BSON.h"

#include <vector>

namespace base
{
	namespace bson
	{
		std::ostream & serialiseInt(std::ostream & out, const int n )
		{
			int t = n;
			out.write( (const char *)&t, sizeof(int) );
			return out;
		}
		std::istream & deserialiseInt(std::istream & in, int &n )
		{
			in.read( ( char *)&n, sizeof(int) );
			return in;
		}


		std::ostream & serialiseString(std::ostream & out, const std::string &str )
		{
			serialiseInt( out, (int)str.size() );
			out.write( str.c_str(), str.size() );
			return out;
		}

		std::istream & deserialiseString(std::istream & in, std::string &str )
		{
			int size = 0;
			deserialiseInt( in, size );
			std::vector<char> tmp;
			tmp.resize( size );
			in.read( (char *)(&tmp[0]), size );
			str = std::string( tmp.begin(), tmp.end() );
			return in;
		}


		// ===============================================
		// Key
		// ===============================================
		Key::Key()
		{
		}

		Key::Key( const std::string &key ) : m_isString(true), m_stringKey(key)
		{
		}

		Key::Key( const int key )
		{
		}

		int Key::hash() const
		{
			std::hash<std::string> test;
			if( m_isString )
				return (int) test(m_stringKey);
			return m_intKey;
		}

		bool operator<( const Key &lhs, const Key &rhs )
		{
			return lhs.hash() < rhs.hash();
		}

		std::ostream & operator<<(std::ostream & out, const Key &key )
		{
			out.write( (char *)(&key.m_isString), sizeof(bool) );
			if(key.m_isString)
				serialiseString(out, key.m_stringKey);
			else
				out.write( (char *)(&key.m_intKey), sizeof(int) );

			return out;
		}
		std::istream & operator>>(std::istream & in, Key &key )
		{
			in.read( (char *)(&key.m_isString), sizeof(bool) );
			if(key.m_isString)
				deserialiseString(in, key.m_stringKey);
			else
				in.read( (char *)(&key.m_intKey), sizeof(int) );

			return in;
		}

		// ===============================================
		// Item
		// ===============================================
		Item::Item() : m_data(0), m_size(0), m_child(BSONPtr())
		{

		}

		std::ostream & operator<<(std::ostream & out, const Item &item )
		{
			bool hasChild = false;
			if(item.m_child)
				hasChild = true;
			out.write( (char *)(&hasChild), sizeof(bool) );
			if( !hasChild )
			{
				out.write( (char*)(&item.m_size), sizeof(int));
				out.write( item.m_data, item.m_size );
			}else
				out << *(item.m_child);

			return out;
		}

		std::istream & operator>>(std::istream & in, Item &item )
		{
			bool hasChild = false;
			in.read( (char *)(&hasChild), sizeof(bool) );
			if( !hasChild )
			{
				in.read( (char *)&item.m_size, sizeof(int) );
				item.m_data = (char *)malloc(sizeof(char)*item.m_size);
				in.read( item.m_data, item.m_size );
			}else
			{
				BSON *child = new BSON();
				in >> *child;
				item.m_child = BSONPtr(child);
			}

			return in;
		}


		// ===============================================
		// ItemHelper
		// ===============================================
		ItemHelper::ItemHelper() : m_bson(BSONPtr())
		{

		}

		ItemHelper::ItemHelper( BSONPtr bson, const Key &key ) : m_bson(bson), m_key(key)
		{
			m_item = m_bson->get(key);
		}

		std::string ItemHelper::asString()
		{
			return std::string( m_item->m_data, m_item->m_size );
		}

		// will only work if wrapped item is a bson object
		ItemHelper ItemHelper::operator[]( const std::string &key )
		{
			if( m_item->m_child )
				return ItemHelper( m_item->m_child, key );
			// TODO:we probably should assert
			return ItemHelper();
		}


		// ===============================================
		// BSON
		// ===============================================
		BSON::BSON() : m_nextIndex(0)
		{
		}

		ItemPtr BSON::get( const Key &key )
		{
			ItemMap::iterator it = m_items.find(key);

			if( it != m_items.end() )
				return it->second;

			return ItemPtr();
		}

		void BSON::set( const Key &key, ItemPtr item )
		{
			m_items[key] = item;
		}

		// uses a local counter
		void BSON::append( ItemPtr item )
		{
			set(m_nextIndex++, item);
		}

		int BSON::numItems() const
		{
			return (int)m_items.size();
		}

		Helper create()
		{
			return Helper(BSONPtr( new BSON() ));
		}


		std::ostream & operator<<(std::ostream & out, const BSON &bson )
		{
			int numItems = (int)bson.m_items.size();
			out.write( (char*)(&numItems), sizeof(int));

			for( BSON::ItemMap::const_iterator it = bson.m_items.begin(); it != bson.m_items.end();++it )
			{
				out << it->first;
				out << *(it->second);
			}

			return out;
		}

		std::istream & operator>>(std::istream & in, BSON &bson )
		{
			int numItems = 0;
			in.read( (char *)&numItems, sizeof(int) );

			for( int i=0;i<numItems;++i )
			{
				Key key;
				Item *item = new Item();
				in >> key;
				in >> *item;
				bson.set( key, ItemPtr(item) );
			}

			return in;
		}


		// ===============================================
		// Helper
		// ===============================================

		ItemHelper Helper::operator[]( const std::string &key )
		{
			return ItemHelper( m_bson, key );
		}


		Helper::Helper( BSONPtr bson ) : m_bson(bson)
		{
		}

		Helper::operator BSONPtr()
		{
			return m_bson;
		}


		// ===============================================
		// Packet
		// ===============================================

		Packet::Packet() : m_data(0), m_size(0)
		{

		}

		Packet::Packet( char *data, int size ) : m_data(data), m_size(size)
		{

		}

		Packet::Packet( const std::string &data )
		{
			m_size = (int)data.size();
			m_data = (char *)malloc(sizeof(char)*m_size);
			memcpy( m_data, data.c_str(), m_size );
		}

		Packet::~Packet()
		{
			if( m_data )
				free(m_data);
			m_size=0;
		}

		PacketPtr pack( Helper &helper )
		{
			std::ostringstream out(std::stringstream::out | std::stringstream::binary);

			out << *(helper.m_bson);
			PacketPtr p = PacketPtr(new Packet(out.str()));
			return p;
		}

		Helper unpack( PacketPtr &packet )
		{
			base::bson::OneShotReadBuf osrb(packet->m_data, packet->m_size);
			std::istream in( &osrb );

			BSON *b = new BSON();

			in >> *b;

			return Helper(BSONPtr( b ));

		}

		Helper unpack( char *data, int size )
		{
			base::bson::OneShotReadBuf osrb(data, size);
			std::istream in( &osrb );

			BSON *b = new BSON();

			in >> *b;

			return Helper(BSONPtr( b ));
		}
	}
}