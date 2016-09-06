/*
MySQL Data Transfer
Source Host: localhost
Source Database: world
Target Host: localhost
Target Database: world
Date: 04/10/2014 14:30:06
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for fishing
-- ----------------------------
CREATE TABLE `fishing` (
  `guid` bigint(10) NOT NULL DEFAULT '0',
  `nbr` bigint(10) DEFAULT '0',
  `name_player` char(10) DEFAULT NULL,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records 
-- ----------------------------
