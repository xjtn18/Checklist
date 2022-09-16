# Checklist (C++ & SFML)

## TODO
	- Create a visual cue for when an item on the checklist has been checked.
	- Have a preallocated vector of animations for each item, so we are not clearing and creating new animations every time we check an item.

## FIXME
	- Checking an item, moving the cursor, and then interrupting the animation before it finishes causes a VSM (visual/state mismatch).


### 9/13/22
	- Brought over the code from VimBell and scrapped everything that was unneccessary / bloated.
	- Created a simple menu of text items and a circular button, which at the moment is non-functional.
	- Currently selected items are highlighted and you can manuever through the checklist using 'j' and 'k'.


### 9/16/22
	- Integrated animations into the menu; items in the checklist now shift appropriately to the bottom of the list when they have been checked.
	- Animations can be intterupted, in which case the previously checked item is reselected and shifted back to the top of the list.
	- Added a new PositionAnimation class that works on Entities rather than just sf::Transformables.

