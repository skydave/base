//
// added myself
//


#ifdef _WINDOWS

#include "ip/TcpSocket.h"

#include <winsock2.h>   // this must come first to prevent errors with MSVC7
#include <windows.h>
#include <mmsystem.h>   // for timeGetTime()

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <assert.h>

#ifndef WINCE
#include <signal.h>
#endif

#include "ip/NetworkingUtils.h"
#include "ip/PacketListener.h"
#include "ip/TimerListener.h"


typedef int socklen_t;

extern void SockaddrFromIpEndpointName( struct sockaddr_in& sockAddr, const IpEndpointName& endpoint );
extern IpEndpointName IpEndpointNameFromSockaddr( const struct sockaddr_in& sockAddr );


class TcpSocket::Implementation
{
    NetworkInitializer networkInitializer_;

	bool isBound_;
	bool isConnected_;

	SOCKET socket_;
	struct sockaddr_in connectedAddr_;
	struct sockaddr_in sendToAddr_;

public:

	Implementation()
		: isBound_( false )
		, isConnected_( false )
		, socket_( INVALID_SOCKET )
	{
		if( (socket_ = socket( AF_INET, SOCK_STREAM, 0 )) == INVALID_SOCKET ){
            throw std::runtime_error("unable to create udp socket\n");
        }

		memset( &sendToAddr_, 0, sizeof(sendToAddr_) );
        sendToAddr_.sin_family = AF_INET;
	}

	Implementation( bool _isBound, bool _isConnected, SOCKET _socket )
		: isBound_( _isBound )
		, isConnected_( _isConnected )
		, socket_( _socket )
	{
	}

	~Implementation()
	{
		if (socket_ != INVALID_SOCKET) closesocket(socket_);
	}

	bool isValid()
	{
		return (socket_ != INVALID_SOCKET);
	}
	/*
	IpEndpointName LocalEndpointFor( const IpEndpointName& remoteEndpoint ) const
	{
		assert( isBound_ );

		// first connect the socket to the remote server
        
        struct sockaddr_in connectSockAddr;
		SockaddrFromIpEndpointName( connectSockAddr, remoteEndpoint );
       
        if (connect(socket_, (struct sockaddr *)&connectSockAddr, sizeof(connectSockAddr)) < 0) {
            throw std::runtime_error("unable to connect udp socket\n");
        }

        // get the address

        struct sockaddr_in sockAddr;
        memset( (char *)&sockAddr, 0, sizeof(sockAddr ) );
        socklen_t length = sizeof(sockAddr);
        if (getsockname(socket_, (struct sockaddr *)&sockAddr, &length) < 0) {
            throw std::runtime_error("unable to getsockname\n");
        }
        
		if( isConnected_ ){
			// reconnect to the connected address
			
			if (connect(socket_, (struct sockaddr *)&connectedAddr_, sizeof(connectedAddr_)) < 0) {
				throw std::runtime_error("unable to connect udp socket\n");
			}

		}else{
			// unconnect from the remote address
		
			struct sockaddr_in unconnectSockAddr;
			SockaddrFromIpEndpointName( unconnectSockAddr, IpEndpointName() );

			if( connect(socket_, (struct sockaddr *)&unconnectSockAddr, sizeof(unconnectSockAddr)) < 0 
					&& WSAGetLastError() != WSAEADDRNOTAVAIL ){
				throw std::runtime_error("unable to un-connect udp socket\n");
			}
		}

		return IpEndpointNameFromSockaddr( sockAddr );
	}
	*/

	void Connect( const IpEndpointName& remoteEndpoint )
	{
		SockaddrFromIpEndpointName( connectedAddr_, remoteEndpoint );
       
		int result = connect(socket_, (struct sockaddr *)&connectedAddr_, sizeof(connectedAddr_));
        if ( result == SOCKET_ERROR) {
            throw std::runtime_error("unable to connect tcp socket\n");
        }

		isConnected_ = true;
	}
	/*


    void SendTo( const IpEndpointName& remoteEndpoint, const char *data, int size )
	{
		sendToAddr_.sin_addr.s_addr = htonl( remoteEndpoint.address );
        sendToAddr_.sin_port = htons( (short)remoteEndpoint.port );

        sendto( socket_, data, size, 0, (sockaddr*)&sendToAddr_, sizeof(sendToAddr_) );
	}
	*/
	void Bind( const IpEndpointName& localEndpoint )
	{
		struct sockaddr_in bindSockAddr;
		SockaddrFromIpEndpointName( bindSockAddr, localEndpoint );

        if (bind(socket_, (struct sockaddr *)&bindSockAddr, sizeof(bindSockAddr))  == SOCKET_ERROR) {
            throw std::runtime_error("unable to bind tcp socket\n");
        }

		isBound_ = true;
	}

	bool IsBound() const { return isBound_; }

	int Listen()
	{
		return listen( socket_, 10 );
	}

	void Accept( TcpSocket &client )
	{
		SOCKET connectedSocket = accept(socket_,NULL,NULL);
		delete client.impl_;
		client.impl_ = new TcpSocket::Implementation( false, true, connectedSocket );
	}
	/*
    int ReceiveFrom( IpEndpointName& remoteEndpoint, char *data, int size )
	{
		assert( isBound_ );

		struct sockaddr_in fromAddr;
        socklen_t fromAddrLen = sizeof(fromAddr);
             	 
        int result = recvfrom(socket_, data, size, 0,
                    (struct sockaddr *) &fromAddr, (socklen_t*)&fromAddrLen);
		if( result < 0 )
			return 0;

		remoteEndpoint.address = ntohl(fromAddr.sin_addr.s_addr);
		remoteEndpoint.port = ntohs(fromAddr.sin_port);

		return result;
	}
	*/

	int Receive( char *data, int size )
	{
		return recv( socket_, data, size, 0);
	}

	void Send( const char *data, int size )
	{
		//assert( isConnected_ );

        send( socket_, data, size, 0 );
	}

	SOCKET& Socket() { return socket_; }
};

TcpSocket::TcpSocket( Implementation *_impl )
{
	impl_ = _impl;
}

TcpSocket::TcpSocket()
{
	impl_ = new Implementation();
}

TcpSocket::~TcpSocket()
{
	delete impl_;
}

/*
IpEndpointName UdpSocket::LocalEndpointFor( const IpEndpointName& remoteEndpoint ) const
{
	return impl_->LocalEndpointFor( remoteEndpoint );
}
*/
void TcpSocket::Connect( const IpEndpointName& remoteEndpoint )
{
	impl_->Connect( remoteEndpoint );
}

int TcpSocket::Receive( char *buffer, int size )
{
	return impl_->Receive( buffer, size );
}

void TcpSocket::Send( const char *data, int size )
{
	impl_->Send( data, size );
}


/*

void UdpSocket::SendTo( const IpEndpointName& remoteEndpoint, const char *data, int size )
{
	impl_->SendTo( remoteEndpoint, data, size );
}
*/
void TcpSocket::Bind( const IpEndpointName& localEndpoint )
{
	impl_->Bind( localEndpoint );
}

bool TcpSocket::IsBound() const
{
	return impl_->IsBound();
}

int TcpSocket::Listen()
{
	return impl_->Listen();
}

void TcpSocket::Accept( TcpSocket &client )
{
	impl_->Accept(client);
}

bool TcpSocket::isValid()
{
	return impl_->isValid();
}
/*
int UdpSocket::ReceiveFrom( IpEndpointName& remoteEndpoint, char *data, int size )
{
	return impl_->ReceiveFrom( remoteEndpoint, data, size );
}


struct AttachedTimerListener{
	AttachedTimerListener( int id, int p, TimerListener *tl )
		: initialDelayMs( id )
		, periodMs( p )
		, listener( tl ) {}
	int initialDelayMs;
	int periodMs;
	TimerListener *listener;
};


static bool CompareScheduledTimerCalls( 
		const std::pair< double, AttachedTimerListener > & lhs, const std::pair< double, AttachedTimerListener > & rhs )
{
	return lhs.first < rhs.first;
}


SocketReceiveMultiplexer *multiplexerInstanceToAbortWithSigInt_ = 0;

extern "C"  void InterruptSignalHandler( int );
void InterruptSignalHandler( int )
{
	multiplexerInstanceToAbortWithSigInt_->AsynchronousBreak();
#ifndef WINCE
    signal( SIGINT, SIG_DFL );
#endif
}


class SocketReceiveMultiplexer::Implementation{
    NetworkInitializer networkInitializer_;

	std::vector< std::pair< PacketListener*, UdpSocket* > > socketListeners_;
	std::vector< AttachedTimerListener > timerListeners_;

	volatile bool break_;
	HANDLE breakEvent_;

	double GetCurrentTimeMs() const
	{
#ifndef WINCE
		return timeGetTime(); // FIXME: bad choice if you want to run for more than 40 days
#else
        return 0;
#endif
    }

public:
    Implementation()
	{
		breakEvent_ = CreateEvent( NULL, FALSE, FALSE, NULL );
	}

    ~Implementation()
	{
		CloseHandle( breakEvent_ );
	}

    void AttachSocketListener( UdpSocket *socket, PacketListener *listener )
	{
		assert( std::find( socketListeners_.begin(), socketListeners_.end(), std::make_pair(listener, socket) ) == socketListeners_.end() );
		// we don't check that the same socket has been added multiple times, even though this is an error
		socketListeners_.push_back( std::make_pair( listener, socket ) );
	}

    void DetachSocketListener( UdpSocket *socket, PacketListener *listener )
	{
		std::vector< std::pair< PacketListener*, UdpSocket* > >::iterator i = 
				std::find( socketListeners_.begin(), socketListeners_.end(), std::make_pair(listener, socket) );
		assert( i != socketListeners_.end() );

		socketListeners_.erase( i );
	}

    void AttachPeriodicTimerListener( int periodMilliseconds, TimerListener *listener )
	{
		timerListeners_.push_back( AttachedTimerListener( periodMilliseconds, periodMilliseconds, listener ) );
	}

	void AttachPeriodicTimerListener( int initialDelayMilliseconds, int periodMilliseconds, TimerListener *listener )
	{
		timerListeners_.push_back( AttachedTimerListener( initialDelayMilliseconds, periodMilliseconds, listener ) );
	}

    void DetachPeriodicTimerListener( TimerListener *listener )
	{
		std::vector< AttachedTimerListener >::iterator i = timerListeners_.begin();
		while( i != timerListeners_.end() ){
			if( i->listener == listener )
				break;
			++i;
		}

		assert( i != timerListeners_.end() );

		timerListeners_.erase( i );
	}

    void Run()
	{
		break_ = false;

		// prepare the window events which we use to wake up on incoming data
		// we use this instead of select() primarily to support the AsyncBreak() 
		// mechanism.

		std::vector<HANDLE> events( socketListeners_.size() + 1, 0 );
		int j=0;
		for( std::vector< std::pair< PacketListener*, UdpSocket* > >::iterator i = socketListeners_.begin();
				i != socketListeners_.end(); ++i, ++j ){

			HANDLE event = CreateEvent( NULL, FALSE, FALSE, NULL );
			WSAEventSelect( i->second->impl_->Socket(), event, FD_READ ); // note that this makes the socket non-blocking which is why we can safely call RecieveFrom() on all sockets below
			events[j] = event;
		}


		events[ socketListeners_.size() ] = breakEvent_; // last event in the collection is the break event

		
		// configure the timer queue
		double currentTimeMs = GetCurrentTimeMs();

		// expiry time ms, listener
		std::vector< std::pair< double, AttachedTimerListener > > timerQueue_;
		for( std::vector< AttachedTimerListener >::iterator i = timerListeners_.begin();
				i != timerListeners_.end(); ++i )
			timerQueue_.push_back( std::make_pair( currentTimeMs + i->initialDelayMs, *i ) );
		std::sort( timerQueue_.begin(), timerQueue_.end(), CompareScheduledTimerCalls );

		const int MAX_BUFFER_SIZE = 4098;
		char *data = new char[ MAX_BUFFER_SIZE ];
		IpEndpointName remoteEndpoint;

		while( !break_ ){

			double currentTimeMs = GetCurrentTimeMs();

            DWORD waitTime = INFINITE;
            if( !timerQueue_.empty() ){

                waitTime = (DWORD)( timerQueue_.front().first >= currentTimeMs
                            ? timerQueue_.front().first - currentTimeMs
                            : 0 );
            }

			DWORD waitResult = WaitForMultipleObjects( (DWORD)socketListeners_.size() + 1, &events[0], FALSE, waitTime );
			if( break_ )
				break;

			if( waitResult != WAIT_TIMEOUT ){
				for( int i = waitResult - WAIT_OBJECT_0; i < (int)socketListeners_.size(); ++i ){
					int size = socketListeners_[i].second->ReceiveFrom( remoteEndpoint, data, MAX_BUFFER_SIZE );
					if( size > 0 ){
						socketListeners_[i].first->ProcessPacket( data, size, remoteEndpoint );
						if( break_ )
							break;
					}
				}
			}

			// execute any expired timers
			currentTimeMs = GetCurrentTimeMs();
			bool resort = false;
			for( std::vector< std::pair< double, AttachedTimerListener > >::iterator i = timerQueue_.begin();
					i != timerQueue_.end() && i->first <= currentTimeMs; ++i ){

				i->second.listener->TimerExpired();
				if( break_ )
					break;

				i->first += i->second.periodMs;
				resort = true;
			}
			if( resort )
				std::sort( timerQueue_.begin(), timerQueue_.end(), CompareScheduledTimerCalls );
		}

		delete [] data;

		// free events
		j = 0;
		for( std::vector< std::pair< PacketListener*, UdpSocket* > >::iterator i = socketListeners_.begin();
				i != socketListeners_.end(); ++i, ++j ){

			WSAEventSelect( i->second->impl_->Socket(), events[j], 0 ); // remove association between socket and event
			CloseHandle( events[j] );
			unsigned long enableNonblocking = 0;
			ioctlsocket( i->second->impl_->Socket(), FIONBIO, &enableNonblocking );  // make the socket blocking again
		}
	}

    void Break()
	{
		break_ = true;
	}

    void AsynchronousBreak()
	{
		break_ = true;
		SetEvent( breakEvent_ );
	}
};



SocketReceiveMultiplexer::SocketReceiveMultiplexer()
{
	impl_ = new Implementation();
}

SocketReceiveMultiplexer::~SocketReceiveMultiplexer()
{	
	delete impl_;
}

void SocketReceiveMultiplexer::AttachSocketListener( UdpSocket *socket, PacketListener *listener )
{
	impl_->AttachSocketListener( socket, listener );
}

void SocketReceiveMultiplexer::DetachSocketListener( UdpSocket *socket, PacketListener *listener )
{
	impl_->DetachSocketListener( socket, listener );
}

void SocketReceiveMultiplexer::AttachPeriodicTimerListener( int periodMilliseconds, TimerListener *listener )
{
	impl_->AttachPeriodicTimerListener( periodMilliseconds, listener );
}

void SocketReceiveMultiplexer::AttachPeriodicTimerListener( int initialDelayMilliseconds, int periodMilliseconds, TimerListener *listener )
{
	impl_->AttachPeriodicTimerListener( initialDelayMilliseconds, periodMilliseconds, listener );
}

void SocketReceiveMultiplexer::DetachPeriodicTimerListener( TimerListener *listener )
{
	impl_->DetachPeriodicTimerListener( listener );
}

void SocketReceiveMultiplexer::Run()
{
	impl_->Run();
}

void SocketReceiveMultiplexer::RunUntilSigInt()
{
	assert( multiplexerInstanceToAbortWithSigInt_ == 0 ); // at present we support only one multiplexer instance running until sig int
	multiplexerInstanceToAbortWithSigInt_ = this;
#ifndef WINCE
    signal( SIGINT, InterruptSignalHandler );
#endif
	impl_->Run();
#ifndef WINCE
	signal( SIGINT, SIG_DFL );
#endif
	multiplexerInstanceToAbortWithSigInt_ = 0;
}

void SocketReceiveMultiplexer::Break()
{
	impl_->Break();
}

void SocketReceiveMultiplexer::AsynchronousBreak()
{
	impl_->AsynchronousBreak();
}
*/

#endif // _WINDOWS
