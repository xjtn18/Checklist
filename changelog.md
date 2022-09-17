# Checklist (C++ & SFML)

## TODO
	- Have a preallocated vector of animations for each item, so we are not clearing and creating new animations every time we check an item.

## FIXME



### 9/13/22
	- Brought over the code from VimBell and scrapped everything that was unneccessary / bloated.
	- Created a simple menu of text items and a circular button, which at the moment is non-functional.
	- Currently selected items are highlighted and you can manuever through the checklist using 'j' and 'k'.


### 9/16/22
	- Integrated animations into the menu; items in the checklist now shift appropriately to the bottom of the list when they have been checked.
	- Animations can be intterupted, in which case the previously checked item is reselected and shifted back to the top of the list.
	- Added a new PositionAnimation class that works on Entities rather than just sf::Transformables.


### 9/16/22
	- Items now have a proper button that visually indicates when it is checked. Checked items' messages are now dimmed.
	- Items now float to the index between the checked and unchecked items, rather than to the top or bottom.

