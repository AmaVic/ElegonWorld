/*
MySQL Data Transfer
Source Host: localhost
Source Database: world
Target Host: localhost
Target Database: world
Date: 04/10/2014 14:30:12
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for fishing_event
-- ----------------------------
CREATE TABLE `fishing_event` (
  `date_start` datetime DEFAULT NULL,
  `date_end` datetime DEFAULT NULL,
  `in_progress` int(10) DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `fishing_event` VALUES ('2014-09-29 00:00:00', '2014-09-30 00:00:00', '0');
