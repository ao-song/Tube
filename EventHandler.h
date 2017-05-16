#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

namespace Tube
{
	class EventHandler
	{
	public:

		
		EventHandler(
		 EventHandlerTable*  theEventHandlerTable,
		 EventHandlerParent* theEventHandlerParent);

		/**
		* Destructor.
		*/
		virtual
		~EventHandler();

		void
		close();

		
		virtual
		bool
		epoll_ctl(unsigned int previousRequestedEvents = 0);

		
		EventHandlerOwner*
		get_owner();

		
		void
		set_owner(eventHandlerOwner* theEventHandlerOwner);

		
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
		 unsigned int theEvent) = 0;

		
		void
		local_address_to_stream(
		 std::ostream& theStream) const;

		
		void
		remote_address_to_stream(
		 std::ostream& theStream) const;

		
		virtual
		void
		reset_requested_events(
		 unsigned int theEventsToReset);

		
		virtual
		void
		set_requested_events(
		 unsigned int theEventsToSet);

		
		void
		set_socket(
		 int theSocket);

		
		virtual
		bool
		is_want_read() const;

		
		virtual
		bool
		is_want_write() const;

		void
		change_event_handler_table(EventHandlerTable* theTable);

		virtual
		void
		remove () = 0;

	private:

		// Copy constructor not implemented.		
		EventHandler(
		 const EventHandler& theOther);

		// Assignment operator not implemented.		
		EventHandler&
		operator=(
		 const EventHandler& theOther);

		void
		address_tostream_impl(
		 std::ostream& theStream,
		 const sockaddr_storage&,
		 socklen_t&) const;

		EventHandlerOwner*  eventHandlerOwnerM;
		EventHandlerTable*  eventHandlerTableM;
		unsigned int        requestedEventsM;
		int                 socketM;
	};
}

#endif