# Checklist (C++ & SFML)

### TODO
	- Lerp the selector movement.
	- Get the text field and text cursor integrated to allow for inline item editing.
	- Add a 'new item' row in the list to add new items dynamically.
	     Have it exist at the end of the currently unchecked items ?
	- Add mouse support

### FIXME
	- figure out why VStack::update must be called in the update method of Menu in order to display the list correctly.
	- Get the text and buttons to line up properly in the Menu no matter the size of the items.


### REFACTOR
	- Add index operator overloading to VStacks/HStacks.



# Log

### 10/7/22
	- Changed color scheme and added some other new colors.
	- Added 'item' method in Menu to get a reference to a specific item object in the list.
	- Move color animations to 'Item' level; Menu did not need to handle that.
	      - Toggle now sets the color animations for the button and text content.
	      - This fixed coloring bug when pressing 'f' to clear all checked items.


### 10/6/22
	- Changed the way animations are processed. Now, every entity has a Position + Color Animation.
	- Deleted functions for processing animations and deleting stale animations in Menu. Entities now carry
	    their respective animations. This makes the code cleaner + the program more efficient.
	- Shifted some method definitions in 'Animation' into a source file to fix circular dependency issue with 'Entity'.


### 10/3/22
	- Finished basic save/load functionality for checklists.


### 10/1/22
	- Moved duplciated code across all animation structs to the base Animation struct.
	- Added virtual 'update' method to reduce redundancy in the way child animations exectued their respective routines.
	- Added constructor to color animation to automically set 'home' to the entity's current color.


### 9/30/22
	- Fixed visual bug of overlapping items when spacing sequential check/uncheck events just right.



### 9/22/22
	- Got concurrent animations working; items no longer rely on positions of other items to compute their destination in a list shift animation. This means users can spam-check items without having to wait for currently running animations and everything is animated accordingly.



### 9/16/22
	- Items now have a proper button that visually indicates when it is checked. Checked items' messages are now dimmed.
	- Items now float to the index between the checked and unchecked items, rather than to the top or bottom.



### 9/16/22
	- Integrated animations into the menu; items in the checklist now shift appropriately to the bottom of the list when they have been checked.
	- Animations can be intterupted, in which case the previously checked item is reselected and shifted back to the top of the list.
	- Added a new PositionAnimation class that works on Entities rather than just sf::Transformables.



### 9/13/22
	- Brought over the code from VimBell and scrapped everything that was unneccessary / bloated.
	- Created a simple menu of text items and a circular button, which at the moment is non-functional.
	- Currently selected items are highlighted and you can manuever through the checklist using 'j' and 'k'.




