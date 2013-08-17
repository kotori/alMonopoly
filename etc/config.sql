-- Specify some sqlite3 specific commands
.headers on
.mode columns
.width 0


-- Drop the table first to prevent conflicting data.
DROP TABLE IF EXISTS `sets`;

-- Create sets table for grouping colors.
CREATE TABLE `sets` (
    `id` INTEGER PRIMARY KEY,
    `name` TEXT
);

-- Populate sets table.
INSERT INTO `sets` VALUES (0, "NULL");
INSERT INTO `sets` VALUES (1, "Purple");
INSERT INTO `sets` VALUES (2, "Light Blue");
INSERT INTO `sets` VALUES (3, "Pink");
INSERT INTO `sets` VALUES (4, "Orange");
INSERT INTO `sets` VALUES (5, "Red");
INSERT INTO `sets` VALUES (6, "Yellow");
INSERT INTO `sets` VALUES (7, "Green");
INSERT INTO `sets` VALUES (8, "Blue");


-- Drop the table first to prevent conflicting data.
DROP TABLE IF EXISTS `types`;

-- Create types table for grouping elements.
CREATE TABLE `types` (
    `id` INTEGER PRIMARY KEY,
    `name` TEXT NOT NULL
);

-- Populate types table.
INSERT INTO `types` VALUES (1, "NULL");
INSERT INTO `types` VALUES (2, "Railroad");
INSERT INTO `types` VALUES (3, "Utility");
INSERT INTO `types` VALUES (4, "Property");
INSERT INTO `types` VALUES (5, "Card");
INSERT INTO `types` VALUES (6, "Tax");


-- Drop the table first to prevent conflicting data.
DROP TABLE IF EXISTS `landing`;

-- Create `landing` table for individual properties.
CREATE TABLE `landing` (
    `id` INTEGER PRIMARY KEY,
    `name` TEXT NOT NULL,
    `price` INTEGER,
    `per_house` INTEGER,
    `per_hotel` INTEGER,
    `mortgage` INTEGER,
    `rent` INTEGER,
    `rent_1_house` INTEGER,
    `rent_2_house` INTEGER,
    `rent_3_house` INTEGER,
    `rent_4_house` INTEGER,
    `rent_hotel` INTEGER,
    `type` INTEGER,
    `set_id` INTEGER
);

-- Populate `landing` table.
INSERT INTO `landing` VALUES (NULL, "Go", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0);
INSERT INTO `landing` VALUES (NULL, "Mediterranean Avenue", 60, 50, 50, 30, 2, 10, 30, 90, 160, 250, 4, 1);
INSERT INTO `landing` VALUES (NULL, "Community Chest", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0);
INSERT INTO `landing` VALUES (NULL, "Baltic Avenue", 60, 50, 50, 30, 4, 20, 60, 180, 320, 450, 4, 1);
INSERT INTO `landing` VALUES (NULL, "Income Tax", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0);
INSERT INTO `landing` VALUES (NULL, "Reading Railroad", 200, 0, 0, 100, 0, 0, 0, 0, 0, 0, 2, 0);
INSERT INTO `landing` VALUES (NULL, "Oriental Avenue", 100, 50, 50, 50, 6, 30, 90, 270, 400, 550, 4, 2);
INSERT INTO `landing` VALUES (NULL, "Chance", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0);
INSERT INTO `landing` VALUES (NULL, "Vermont Avenue", 100, 50, 50, 50, 6, 30, 90, 270, 400, 550, 4, 2);
INSERT INTO `landing` VALUES (NULL, "Conneticut Avenue", 120, 50, 50, 60, 8, 40, 100, 350, 450, 600, 4, 2);
INSERT INTO `landing` VALUES (NULL, "Jail", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0);
INSERT INTO `landing` VALUES (NULL, "St. Charles Place", 140, 100, 100, 70, 10, 50, 150, 450, 625, 750, 4, 3);
INSERT INTO `landing` VALUES (NULL, "Electric Company", 150, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0);
INSERT INTO `landing` VALUES (NULL, "States Avenue", 140, 100, 100, 70, 10, 50, 150, 450, 625, 750, 4, 3);
INSERT INTO `landing` VALUES (NULL, "Virgina Avenue", 160, 100, 100, 80, 12, 60, 180, 500, 700, 900, 4, 3);
INSERT INTO `landing` VALUES (NULL, "Pennsylvania Railroad", 200, 0, 0, 100, 0, 0, 0, 0, 0, 0, 2, 0);
INSERT INTO `landing` VALUES (NULL, "St. James Place", 180, 100, 100, 90, 14, 70, 200, 550, 750, 950, 4, 4);
INSERT INTO `landing` VALUES (NULL, "Community Chest", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0);
INSERT INTO `landing` VALUES (NULL, "Tennessee Avenue", 180, 100, 100, 90, 14, 70, 200, 550, 750, 950, 4, 4);
INSERT INTO `landing` VALUES (NULL, "New York Avenue", 200, 100, 100, 100, 16, 80, 220, 600, 800, 1000, 4, 4);
INSERT INTO `landing` VALUES (NULL, "Free Parking", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0);
INSERT INTO `landing` VALUES (NULL, "Kentucky Avenue", 220, 150, 150, 110, 18, 90, 250, 700, 875, 1050, 4, 5);
INSERT INTO `landing` VALUES (NULL, "Chance", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0);
INSERT INTO `landing` VALUES (NULL, "Indiana Avenue", 220, 150, 150, 110, 18, 90, 250, 700, 875, 1050, 4, 5);
INSERT INTO `landing` VALUES (NULL, "Illinois Avenue", 240, 150, 150, 120, 20, 100, 300, 750, 925, 1100, 4, 5);
INSERT INTO `landing` VALUES (NULL, "B&O Railroad", 200, 0, 0, 100, 0, 0, 0, 0, 0, 0, 2, 0);
INSERT INTO `landing` VALUES (NULL, "Atlantic Avenue", 260, 150, 150, 130, 22, 110, 330, 800, 975, 1150, 4, 6);
INSERT INTO `landing` VALUES (NULL, "Ventor Avenue", 260, 150, 150, 130, 22, 110, 330, 800, 975, 1150, 4, 6);
INSERT INTO `landing` VALUES (NULL, "Water Works", 150, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0);
INSERT INTO `landing` VALUES (NULL, "Marvin Gardens", 280, 150, 150, 140, 24, 120, 360, 850, 1025, 1200, 4, 6);
INSERT INTO `landing` VALUES (NULL, "Go To Jail", 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 1, 0);
INSERT INTO `landing` VALUES (NULL, "Pacific Avenue", 300, 200, 200, 150, 26, 130, 390, 900, 1100, 1275, 4, 7);
INSERT INTO `landing` VALUES (NULL, "North Carolina Avenue", 300, 200, 200, 150, 26, 130, 390, 900, 1100, 1275, 4, 7);
INSERT INTO `landing` VALUES (NULL, "Community Chest", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0);
INSERT INTO `landing` VALUES (NULL, "Pennsylvania Avenue", 320, 200, 200, 160, 28, 150, 450, 1000, 1200, 1400, 4, 7);
INSERT INTO `landing` VALUES (NULL, "Shortline Railroad", 200, 0, 0, 100, 0, 0, 0, 0, 0, 0, 2, 0);
INSERT INTO `landing` VALUES (NULL, "Chance", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0);
INSERT INTO `landing` VALUES (NULL, "Park Place", 350, 200, 200, 175, 35, 175, 500, 1100, 1300, 1500, 4, 8);
INSERT INTO `landing` VALUES (NULL, "Luxury Tax", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0);
INSERT INTO `landing` VALUES (NULL, "Boardwalk", 400, 200, 200, 200, 50, 200, 600, 1400, 1700, 2000, 4, 8);


-- Print the contents of types
SELECT * FROM `types`;

-- Print the contents of landing
SELECT * FROM  `landing` ;

