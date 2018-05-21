# LIS-770i project code organization
Separate project code into sub-folders by board name:

- simBrd
- mBrd

Libraries are shared:

- libraries are not in a project folder
- the libraries are developed in their own folder called `lib`
- the projects link against the object files in `lib`
