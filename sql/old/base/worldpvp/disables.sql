-- --------------------------------------------------------
-- Host:                         logon.hellscream.org
-- Server version:               5.5.9-log - MySQL Community Server (GPL)
-- Server OS:                    Win32
-- HeidiSQL Version:             10.3.0.5771
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

-- Dumping structure for table worldpvp.disables
CREATE TABLE IF NOT EXISTS `disables` (
  `sourceType` int(10) unsigned NOT NULL,
  `entry` int(10) unsigned NOT NULL,
  `flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `params_0` varchar(255) NOT NULL DEFAULT '',
  `params_1` varchar(255) NOT NULL DEFAULT '',
  `comment` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`sourceType`,`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table worldpvp.disables: 156 rows
DELETE FROM `disables`;
/*!40000 ALTER TABLE `disables` DISABLE KEYS */;
INSERT INTO `disables` (`sourceType`, `entry`, `flags`, `params_0`, `params_1`, `comment`) VALUES
	(1, 10871, 0, '', '', ''),
	(0, 97670, 1, '', '', 'Item_69240_bug_use'),
	(1, 1318, 0, '', '', 'Unfinished Gordok Business replaced by 7703'),
	(1, 9322, 0, '0', '0', 'Quest: Wild Fires in Kalimdor, removed in patch 2.4.0'),
	(1, 9386, 0, '0', '0', 'Quest: A Light in Dark Places {Daily}, removed in patch 2.4.0'),
	(1, 9319, 0, '0', '0', 'Quest: A Light in Dark Places, removed in patch 2.4.0'),
	(1, 9368, 0, '0', '0', 'Quest: The Festival of Fire {H}, removed in patch 2.4.0'),
	(1, 9367, 0, '0', '0', 'Quest: The Festival of Fire {A}, removed in patch 2.4.0'),
	(0, 89089, 1, '', '', 'Pelle instant kill need condition'),
	(1, 744, 8, '', '', 'Poison spider spell buggue'),
	(0, 40120, 49, '489, 529, 30, 566, 607, 628, 761, 726', '', 'Swift Flight Form in Battlegrounds'),
	(1, 26826, 0, '', '', 'Dask "The Flask" Gobfizzle - Quest Duplicate'),
	(0, 123164, 1, '', '', 'freeze on raid group'),
	(0, 31696, 64, '', '', 'Ignore LOS on Thane'),
	(1, 26867, 0, '', '', 'Enemies Below - Obsolete'),
	(1, 11373, 0, '', '', 'Decrapeted quest: Wanted: Shaffar\'s Wondrous Pendant'),
	(1, 29831, 0, '', '', 'DEPRECATED: Terror on the High Seas'),
	(1, 29832, 0, '', '', 'DEPRECATED: Gathering Evidence'),
	(1, 29845, 0, '', '', 'DEPRECATED: The Merchants\' Plight'),
	(1, 29846, 0, '', '', 'DEPRECATED: Captain Rufus Hardwick'),
	(1, 29849, 0, '', '', 'DEPRECATED: Gunner Blastbarrel'),
	(1, 29852, 0, '', '', 'DEPRECATED: They Call Him Swifthands'),
	(1, 29854, 0, '', '', 'DEPRECATED: Claiming Kalimdor'),
	(1, 29857, 0, '', '', 'DEPRECATED: Where Are Those Ships?'),
	(1, 29867, 0, '', '', 'DEPRECATED: Shadow Hunter Urko\'jin'),
	(1, 29868, 0, '', '', 'DEPRECATED: Captain Cartheron'),
	(1, 29876, 0, '', '', 'DEPRECATED: The Third Captain'),
	(1, 30215, 0, '', '', 'DEPRECATED'),
	(1, 30216, 0, '', '', 'DEPRECATED'),
	(1, 30217, 0, '', '', 'DEPRECATED'),
	(1, 30218, 0, '', '', 'DEPRECATED'),
	(1, 30219, 0, '', '', 'DEPRECATED'),
	(1, 30220, 0, '', '', 'DEPRECATED'),
	(1, 30221, 0, '', '', 'DEPRECATED'),
	(1, 30222, 0, '', '', 'DEPRECATED'),
	(1, 30223, 0, '', '', 'DEPRECATED'),
	(1, 30224, 0, '', '', 'DEPRECATED'),
	(0, 124840, 8, '', '', 'hunters exploit with pet'),
	(0, 46642, 1, '', '', 'GM Abuse - Spell : 5,000 Gold'),
	(4, 17576, 0, '', '', 'Shannox - Backet List'),
	(4, 17775, 0, '', '', 'Shannox - Backet List'),
	(4, 17776, 0, '', '', 'Shannox - Backet List'),
	(4, 17777, 0, '', '', 'Shannox - Backet List'),
	(4, 17778, 0, '', '', 'Shannox - Backet List'),
	(4, 17779, 0, '', '', 'Shannox - Backet List'),
	(0, 46171, 64, '', '', 'Ignore LOS on Scuttle Wrecked Flying Machine'),
	(0, 147055, 1, '', '', 'Singing Crystal - Need to be used ONLY on Timeless Isle creatures'),
	(1, 38296, 0, '0', '0', 'Quest disabled because causing farm'),
	(1, 30231, 0, '', '', 'Récolte de pommettes'),
	(0, 127249, 49, '489, 529, 30, 566, 607, 628, 761,726,559,562,572,617,618,980,1134', '0', 'disable toy for battleground Spell Flesh to Stone'),
	(0, 137270, 1, '', '', 'Charged Moganite - Voice of the Thunder King - Need to be fixed'),
	(1, 9323, 0, '0', '0', 'Quest: Wild Fires in the Eastern Kingdoms, removed in patch 2.4.0'),
	(1, 9388, 0, '0', '0', 'Quest: Flickering Flames in Kalimdor, removed in patch 2.4.0'),
	(1, 9389, 0, '0', '0', 'Quest: Flickering Flames in the Eastern Kingdoms, removed in patch 2.4.0'),
	(1, 7671, 0, '', '', 'Périmé'),
	(1, 7672, 0, '', '', 'Périmé'),
	(1, 7664, 0, '', '', 'Périmé'),
	(1, 7665, 0, '', '', 'Périmé'),
	(0, 148385, 1, '', '', 'Spell Censer of Eternal Agony Usebug tempfix'),
	(0, 77682, 1, '', '', 'Hurl Net - Stun players - Need conditions on NPC 41520'),
	(1, 31536, 0, '', '', 'abuse'),
	(0, 172365, 1, '', '', 'Spell Abuse (summon beatface)'),
	(4, 22935, 0, '', '', 'RAF achievement'),
	(4, 22936, 0, '', '', 'RAF achievement'),
	(0, 172380, 1, '', '', 'Spell Abuse (summon beatface)'),
	(0, 165805, 1, '', '', 'Spell Abuse (summon beatface)'),
	(0, 155941, 11, '', '', 'Summon Maraad not used'),
	(0, 127278, 49, '489, 529, 30, 566, 607, 628, 761,726,559,562,572,617,618,980,1134', '0', 'Disable iwaterwalk mount in pvpzone'),
	(0, 127271, 49, '489, 529, 30, 566, 607, 628, 761,726,559,562,572,617,618,980,1134', '0', 'Disable iwaterwalk mount in pvpzone'),
	(0, 118089, 48, '489, 529, 30, 566, 607, 628, 761,726,559,562,572,617,618,980', '0', 'Disable waterwalk mount in pvpzone'),
	(0, 92146, 49, '489, 529, 30, 566, 607, 628, 761,726,559,562,572,617,618,980,1134', '0', 'Disable toy 64881 in pvp zones'),
	(1, 120472, 23, '0, 1, 1116', '', ''),
	(1, 115811, 23, '0, 1, 1116', '', ''),
	(1, 115817, 23, '0, 1, 1116', '', ''),
	(1, 115856, 23, '0, 1, 1116', '', ''),
	(0, 156803, 19, '0 1', '', ''),
	(0, 115856, 19, '0 1', '', ''),
	(0, 127131, 16, '1519', '', 'Disable yunque in Stormwind zone,exploit'),
	(1, 0, 0, '', '', ''),
	(7, 457, 0, '', '', 'Garrison Mission Not Available (Blackhand\'s Crucible)'),
	(0, 165152, 2, '', '', ''),
	(0, 125634, 1, '', '', 'Call for Nomi - Temp disable'),
	(7, 456, 0, '', '', 'Garrison Mission Not Available (Iron Assembly)'),
	(1, 2158, 0, '0', '0', 'Quest Rest and Relaxation - void since Cataclysm'),
	(1, 38293, 0, '0', '0', 'Quest disabled because causing farm'),
	(0, 139736, 1, '0', '0', 'Activate every game object'),
	(0, 88914, 32, '1', '0', 'Disable item 63092 because a exploit in bgs'),
	(7, 313, 0, '', '', 'Garrison Mission Not Available (Highmaul Raid)'),
	(0, 32307, 49, '489, 529, 30, 566, 607, 628, 761,726,559,562,572,617,618,980,1134', '0', 'Disable item 25552 in pvp zones'),
	(7, 314, 0, '', '', 'Garrison Mission Not Available (Highmaul Raid)'),
	(7, 315, 0, '', '', 'Garrison Mission Not Available (Highmaul Raid)'),
	(7, 455, 0, '', '', 'Garrison Mission Not Available (Black Forge)'),
	(7, 453, 0, '', '', 'Garrison Mission Not Available (Blackhand\'s Crucible)'),
	(7, 452, 0, '', '', 'Garrison Mission Not Available (Iron Assembly)'),
	(7, 451, 0, '', '', 'Garrison Mission Not Available (Black Forge)'),
	(7, 450, 0, '', '', 'Garrison Mission Not Available (Slagworks)'),
	(7, 449, 0, '', '', 'Garrison Mission Not Available (Blackhand\'s Crucible)'),
	(7, 448, 0, '', '', 'Garrison Mission Not Available (Iron Assembly)'),
	(7, 447, 0, '', '', 'Garrison Mission Not Available (Black Forge)'),
	(7, 325, 0, '', '', 'Garrison Mission Not Available (Highmaul Raid)'),
	(7, 446, 0, '', '', 'Garrison Mission Not Available (Slagworks)'),
	(7, 430, 0, '', '', 'Garrison Mission Not Available (Blackhand\'s Crucible)'),
	(7, 429, 0, '', '', 'Garrison Mission Not Available (Iron Assembly)'),
	(7, 428, 0, '', '', 'Garrison Mission Not Available (Black Forge)'),
	(7, 427, 0, '', '', 'Garrison Mission Not Available (Slagworks)'),
	(7, 328, 0, '', '', 'Garrison Mission Not Available (Highmaul Raid)'),
	(7, 327, 0, '', '', 'Garrison Mission Not Available (Highmaul Raid)'),
	(7, 326, 0, '', '', 'Garrison Mission Not Available (Highmaul Raid)'),
	(7, 324, 0, '', '', 'Garrison Mission Not Available (Highmaul Raid)'),
	(7, 323, 0, '', '', 'Garrison Mission Not Available (Highmaul Raid)'),
	(7, 322, 0, '', '', 'Garrison Mission Not Available (Highmaul Raid)'),
	(7, 454, 0, '', '', 'Garrison Mission Not Available (Slagworks)'),
	(7, 321, 0, '', '', 'Garrison Mission Not Available (Highmaul Raid)'),
	(7, 320, 0, '', '', 'Garrison Mission Not Available (Highmaul Raid)'),
	(7, 319, 0, '', '', 'Garrison Mission Not Available (Highmaul Raid)'),
	(0, 11416, 49, '489, 529, 30, 566, 607, 628, 761,726,559,562,572,617,618,980,1134', '0', 'Disable Portal: Ironforge'),
	(7, 318, 0, '', '', 'Garrison Mission Not Available (Highmaul Raid)'),
	(7, 317, 0, '', '', 'Garrison Mission Not Available (Highmaul Raid)'),
	(0, 32266, 49, '489, 529, 30, 566, 607, 628, 761,726,559,562,572,617,618,980,1134', '0', 'Disable Portal: Exodar'),
	(0, 49360, 49, '489, 529, 30, 566, 607, 628, 761,726,559,562,572,617,618,980,1134', '0', 'Disable Portal: Theramore'),
	(0, 11417, 49, '489, 529, 30, 566, 607, 628, 761,726,559,562,572,617,618,980,1134', '0', 'Disable Portal: Orgrimmar'),
	(0, 11418, 49, '489, 529, 30, 566, 607, 628, 761,726,559,562,572,617,618,980,1134', '0', 'Disable Portal: Undercity'),
	(0, 11420, 49, '489, 529, 30, 566, 607, 628, 761,726,559,562,572,617,618,980,1134', '0', 'Disable Portal: Tunder Bluff'),
	(7, 316, 0, '', '', 'Garrison Mission Not Available (Highmaul Raid)'),
	(0, 49361, 49, '489, 529, 30, 566, 607, 628, 761,726,559,562,572,617,618,980,1134', '0', 'Disable Portal: Stonard'),
	(0, 35717, 49, '489, 529, 30, 566, 607, 628, 761,726,559,562,572,617,618,980,1134', '0', 'Disable Portal: Shattrath'),
	(0, 53142, 49, '489, 529, 30, 566, 607, 628, 761,726,559,562,572,617,618,980,1134', '0', 'Disable Portal: Dalaran'),
	(0, 88345, 49, '489, 529, 30, 566, 607, 628, 761,726,559,562,572,617,618,980,1134', '0', 'Disable Portal: Tol Barad'),
	(0, 120146, 49, '489, 529, 30, 566, 607, 628, 761,726,559,562,572,617,618,980,1134', '0', 'Disable Ancient Portal: Dalaran'),
	(1, 36033, 0, '', '', 'Quest : Reputation points abuse for autocomplete quest'),
	(1, 36042, 0, '', '', 'Quest : Reputation points abuse for autocomplete quest'),
	(1, 36242, 0, '', '', 'Garrison Invasion: Where There Is Smoke (H)'),
	(1, 36161, 0, '', '', 'Garrison Invasion: Where There Is Smoke (A)'),
	(1, 35142, 0, '', '', 'Garrison Invasion: Goren Invasion! (H)'),
	(1, 35935, 0, '', '', 'Garrison Invasion: Iron Horde Invasion! (H)'),
	(1, 36414, 0, '', '', 'Garrison Invasion: Shadowmoon Invasion! (H)'),
	(1, 36623, 0, '', '', 'Garrison Invasion: Ogre Invasion! (H)'),
	(1, 36653, 0, '', '', 'Garrison Invasion: Botani Invasion! (H)'),
	(1, 36831, 0, '', '', 'Garrison Invasion: Shadow Council Invasion! (H)'),
	(0, 73523, 7, '', '', 'rigor disable undead'),
	(1, 3116, 0, '', '', 'Deprecated quest: Simple Sigil'),
	(1, 3117, 0, '', '', 'Deprecated quest: Etched Sigil'),
	(1, 3118, 0, '', '', 'Deprecated quest: Encrypted Sigil'),
	(1, 3119, 0, '', '', 'Deprecated quest: Hallowed Sigil'),
	(1, 3120, 0, '', '', 'Deprecated quest: Verdant Sigil'),
	(1, 26945, 0, '', '', 'Deprecated quest: Learning New Techniques'),
	(1, 26946, 0, '', '', 'Deprecated quest: A Rogue\'s Advantage'),
	(1, 26947, 0, '', '', 'Deprecated quest: A Woodsman\'s Training'),
	(1, 26948, 0, '', '', 'Deprecated quest: Moonfire'),
	(1, 26949, 0, '', '', 'Deprecated quest: Learning the Word'),
	(0, 126892, 7, '', '', '1-60 spell 126892/zen-pilgrimage'),
	(1, 7632, 0, '', '', 'Quest - The Ancient Leaf - Deprecated in 4.0.3'),
	(0, 46393, 7, '', '', '46393/brutal-assault'),
	(0, 46392, 7, '', '', '46392/focused-assault'),
	(0, 26013, 7, '', '', 'Disabled [Deserter] bg spell as not nededed with merc mode.'),
	(0, 40775, 7, '', '', 'https://www.wowhead.com/spell=40775/stun-zone');
/*!40000 ALTER TABLE `disables` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
