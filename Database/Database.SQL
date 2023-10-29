SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `Eindopdracht
--

-- --------------------------------------------------------


--
-- Table structure for table `LAnd`
--

CREATE TABLE IF NOT EXISTS `tblLand` (
  `ID` int NOT NULL,
  `strLand` varchar(20) NOT NULL,
  PRIMARY KEY (`ID`),
  KEY (`strLand`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Table structure for table `Stad`
--

CREATE TABLE IF NOT EXISTS `tblStad` (
  `ID` int NOT NULL,
  `strStad` varchar(20) NOT NULL,
  PRIMARY KEY (`ID`),
  KEY (`strStad`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


--
-- Table structure for table `Locatie`
--

CREATE TABLE IF NOT EXISTS `tblLocatie` (
  `ID` int NOT NULL,
  `StadID` int NOT NULL,
  `LandID` int NOT NULL,
  PRIMARY KEY (`ID`),
  FOREIGN KEY(`StadID`) REFERENCES `tblStad`(`ID`) ON UPDATE CASCADE ON DELETE CASCADE,
  FOREIGN KEY(`LandID`) REFERENCES `tblLand`(`ID`) ON UPDATE CASCADE ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Table structure for table `Merk`
--

CREATE TABLE IF NOT EXISTS `tblMerk` (
  `ID` int NOT NULL,
  `strMerk` varchar(20) NOT NULL,
  `LocatieID` int NOT NULL,
  `Plaatje` BLOB,
  PRIMARY KEY (`ID`),
  FOREIGN KEY(`LocatieID`) REFERENCES `tblLocatie`(`ID`)ON UPDATE CASCADE ON DELETE CASCADE,
  KEY (`strMerk`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Table structure for table `Uitvoering`
--

CREATE TABLE IF NOT EXISTS `tblUitvoering` (
  `ID` int NOT NULL,
  `strUitvoering` varchar(20) NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Table structure for table `Type`
--

CREATE TABLE IF NOT EXISTS `tblType` (
  `ID` int NOT NULL,
  `strType` varchar(20) NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


--
-- Table structure for table `Model`
--

CREATE TABLE IF NOT EXISTS `tblModel` (
  `ID` int NOT NULL,
  `UitvoeringID` int,
  `TypeID` int,
  `Vermogen` int,
  PRIMARY KEY (`ID`),
  FOREIGN KEY(`UitvoeringID`) REFERENCES `tblUitvoering`(`ID`)ON UPDATE CASCADE ON DELETE CASCADE,
  FOREIGN KEY(`TypeID`) REFERENCES `tblType`(`ID`)ON UPDATE CASCADE ON DELETE CASCADE
 ) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Table structure for table `Auto`
--

CREATE TABLE IF NOT EXISTS `tblAuto` (
  `ID` int NOT NULL,
  `TypeID` int NOT NULL,
  `MerkID` int NOT NULL,
  PRIMARY KEY (`ID`),
  FOREIGN KEY(`TypeID`) REFERENCES `tblModel`(`ID`)ON UPDATE CASCADE ON DELETE CASCADE,
  FOREIGN KEY(`MerkID`) REFERENCES `tblMerk`(`ID`) ON UPDATE CASCADE ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
