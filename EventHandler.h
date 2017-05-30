#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

namespace Tube
{
	class EventHandler
	{
	public:
		
		EventHandler(
		    EventHandlerTable*  eventHandlerTable,
            EventHandlerOwner*  eventHandlerOwner);

		/**
		* Destructor.
		*/
		virtual
		~EventHandler();

		void
		close();

		
		virtual
		bool
		epoll_ctl(
			unsigned int previousRequestedEvents = 0);

		
		EventHandlerTable*
		get_table();

		
		int
		get_socket() const;

		
		virtual
		unsigned int
		get_request_events() const;

		
		virtual
		bool
		handle_event(
		    unsigned int event) = 0;

		
		void
		local_address_to_stream(
		    std::ostream& stream) const;

		
		void
		remote_address_to_stream(
		    std::ostream& stream) const;

		
		virtual
		void
		reset_requested_events(
		    unsigned int eventsToReset);

		
		virtual
		void
		set_requested_events(
		    unsigned int eventsToSet);

		
		void
		set_socket(
		    int socket);

		
		virtual
		bool
		is_want_read() const;

		
		virtual
		bool
		is_want_write() const;

		void
		change_event_handler_table(
			EventHandlerTable* table);

		virtual
		void
		remove () = 0;

	private:

		// Copy constructor not implemented.		
		EventHandler(
		    const EventHandler& other);

		// Assignment operator not implemented.		
		EventHandler&
		operator=(
		    const EventHandler& other);

		void
		address_tostream_impl(
		    std::ostream& stream,
		    const sockaddr_storage&,
		    socklen_t&) const;

		EventHandlerOwner*  eventHandlerOwnerM;
		EventHandlerTable*  eventHandlerTableM;
		unsigned int        requestedEventsM;
		int                 socketM;
	};
}

#endif