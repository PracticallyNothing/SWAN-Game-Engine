#ifndef SWAN_EVENT_MANAGER_HPP
#define SWAN_EVENT_MANAGER_HPP

#include <vector> // For std::vector<T>
#include <initializer_list> // For std::initializer_list<T>

#include "EventListener.hpp" // For SWAN::EventListener

namespace SWAN {
	class EventManager {
		template <typename T>
			using Vector = std::vector<T>;

		template <typename T>
			using InitList = std::initializer_list<T>;

		public:
			EventManager() {}
			explicit EventManager(InitList<EventListener> il) : listeners(il) {}

			void addListener(EventListener el) { listeners.push_back(el); }
			void update() { for(const auto& l : listeners) l.update(); }

			template <typename Predicate>
			void updateIf(Predicate&& p) {
				for(const auto& l : listeners)
					if(p(l))
						update();
			}

			inline void updateByType(const std::string& type){
				updateIf([type&](const EventListener& l) -> bool { return l.type == type });
			}

			// TODO: Avoid using functions which may throw an exception,
			//       they are a risk to performance.
			inline auto getListenerByIndex(int index) { return listeners.at(index); }

			/// Returns all EventListeners that can currently activate
			inline auto getActiveListeners() {
				return getListenersByPredicate([](const EventListener& l) -> bool { return l.active() && !l.expired(); });
			}

			/// Returns all EventListeners that have expired
			inline auto getExpiredListeners() {
				return getListenersByPredicate([](const EventListener& l) -> bool { return l.expired(); });
			}

			/// Returns all EventListeners that have a matching type
			inline auto getListenersByType(const std::string& type){
				return getListenersByPredicate([type&](const EventListener& l) -> bool { return l.type == type; });
			}

			/// Returns EventListeners based on predicate
			template <typename Predicate>
			Vector<EventListener> getListenersByPredicate(Predicate&& p){
				Vector<EventListener> res; 

				for(const auto& l : listeners)
					if(p(l))
						res.push_back(l);
				
				return res;
			}
		private:
			Vector<EventListener> listeners;
	};
}

#endif // SWAN_EVENT_MANAGER_HPP
