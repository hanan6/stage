-- phpMyAdmin SQL Dump
-- version 3.4.10.1deb1
-- http://www.phpmyadmin.net
--
-- Client: localhost
-- Généré le : Sam 02 Mai 2015 à 19:24
-- Version du serveur: 5.5.41
-- Version de PHP: 5.3.10-1ubuntu3.17

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Base de données: `modele`
--

-- --------------------------------------------------------

--
-- Structure de la table `cardinality`
--

CREATE TABLE IF NOT EXISTS `cardinality` (
  `id_ca` int(11) NOT NULL AUTO_INCREMENT,
  `min` int(11) NOT NULL,
  `max` int(11) NOT NULL,
  PRIMARY KEY (`id_ca`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Structure de la table `embedding`
--

CREATE TABLE IF NOT EXISTS `embedding` (
  `id_em` int(11) NOT NULL AUTO_INCREMENT,
  `min` int(11) NOT NULL,
  `max` int(11) NOT NULL,
  PRIMARY KEY (`id_em`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Structure de la table `lengthBlock`
--

CREATE TABLE IF NOT EXISTS `lengthBlock` (
  `id_lb` int(11) NOT NULL AUTO_INCREMENT,
  `min` int(11) NOT NULL,
  `max` int(11) NOT NULL,
  PRIMARY KEY (`id_lb`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Structure de la table `lengthMotif`
--

CREATE TABLE IF NOT EXISTS `lengthMotif` (
  `id_lm` int(11) NOT NULL AUTO_INCREMENT,
  `min` int(11) NOT NULL,
  `max` int(11) NOT NULL,
  PRIMARY KEY (`id_lm`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Structure de la table `modele`
--

CREATE TABLE IF NOT EXISTS `modele` (
  `id_mod` int(11) NOT NULL AUTO_INCREMENT,
  `id_pos` int(11) NOT NULL,
  `id_neg` int(11) NOT NULL,
  PRIMARY KEY (`id_mod`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Structure de la table `negative`
--

CREATE TABLE IF NOT EXISTS `negative` (
  `id_neg` int(100) NOT NULL AUTO_INCREMENT,
  `p-maximality` tinyint(1) NOT NULL,
  `e-maximality` tinyint(1) NOT NULL,
  `lengthBlock` int(11) NOT NULL,
  `slackBlock` int(11) NOT NULL,
  `embedding` int(11) NOT NULL,
  `patternREG` varchar(100) NOT NULL,
  `p-free` tinyint(1) NOT NULL,
  `e-free` tinyint(1) NOT NULL,
  `partialOrder` enum('Embedding') NOT NULL,
  `totalOrder` enum('Embedding') NOT NULL,
  `lengthMotif` int(11) NOT NULL,
  `slackMotif` int(11) NOT NULL,
  `cardinality` int(11) NOT NULL,
  PRIMARY KEY (`id_neg`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Structure de la table `positive`
--

CREATE TABLE IF NOT EXISTS `positive` (
  `id_pos` int(100) NOT NULL AUTO_INCREMENT,
  `p-maximality` tinyint(1) NOT NULL,
  `e-maximality` tinyint(1) NOT NULL,
  `lengthBlock` int(11) NOT NULL,
  `slackBlock` int(11) NOT NULL,
  `embedding` int(11) NOT NULL,
  `patternREG` varchar(100) NOT NULL,
  `p-free` tinyint(1) NOT NULL,
  `e-free` tinyint(1) NOT NULL,
  `partialOrder` enum('Embedding') NOT NULL,
  `totalOrder` enum('Embedding') NOT NULL,
  `lengthMotif` int(11) NOT NULL,
  `slackMotif` int(11) NOT NULL,
  `cardinality` int(11) NOT NULL,
  PRIMARY KEY (`id_pos`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Structure de la table `slackBlock`
--

CREATE TABLE IF NOT EXISTS `slackBlock` (
  `id_sb` int(11) NOT NULL AUTO_INCREMENT,
  `min` int(11) NOT NULL,
  `max` int(11) NOT NULL,
  PRIMARY KEY (`id_sb`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Structure de la table `slackMotif`
--

CREATE TABLE IF NOT EXISTS `slackMotif` (
  `id_sm` int(11) NOT NULL AUTO_INCREMENT,
  `min` int(11) NOT NULL,
  `max` int(11) NOT NULL,
  PRIMARY KEY (`id_sm`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
