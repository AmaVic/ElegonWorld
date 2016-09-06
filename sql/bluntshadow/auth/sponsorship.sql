/*
SQLyog Community v11.51 (64 bit)
MySQL - 5.6.19 
*********************************************************************
*/
/*!40101 SET NAMES utf8 */;

create table `sponsorship` (
	`sponsorAccountId` int (10),
	`sponsoredAccountId` int (10),
	`startDate` date ,
	`endDate` date 
);
