/*
SQLyog Community v11.51 (64 bit)
MySQL - 5.6.19 : Database - ew_characters
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`ew_characters` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `ew_characters`;

/*Table structure for table `bgscores` */

DROP TABLE IF EXISTS `bgscores`;

CREATE TABLE `bgscores` (
  `playerGUID` int(10) NOT NULL,
  `bgID` int(10) unsigned NOT NULL,
  `victories` int(10) unsigned NOT NULL DEFAULT '0',
  `defeats` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`playerGUID`,`bgID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
