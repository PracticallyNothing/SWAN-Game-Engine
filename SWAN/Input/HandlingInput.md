# Handling input

In SWAN, input is handled in 3 parts:
- The event queue
- The flat input representation
- Input frames

## The event queue
The event queue is an ordered list of events that have occured.
An event (SWAN::Event) may be anything from a key press to the window getting resized (See SWAN::EventType for a full list of possible events).
The event queue is updated each time a call to SWAN::UpdateInputEvents() is made.

_The event queue shouldn't matter to you._ It will just sit in the background and do its job of getting filled and processed on its own - all you have to do is call SWAN::UpdateEvents(). If you want to handle the events, you'll need an input frame.

## The flat input representation
The flat input representation (SWAN::FlatInputRep) is a flattened version of what the event queue sees: it knows if an event has occured, but does not know the order of events. It can be used to query information that the current event doesn't report (e.g. a mouse button was pressed, but you want to know where it was pressed).

## Input frames

An input frame represents and object that can respond to input events.
This object may be anything from the GUI to the actual game itself or even a simple text box.
Input events means anything that the user of the actual program can do - moving the mouse, pressing a key, etc.

For example, the class SWAN::GUIManager implements the SWAN::InputFrame interface, and thus can respond to the mouse, the keyboard and so on. 

