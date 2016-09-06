# Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

# file(GLOB_RECURSE sources_Custom Custom/*.cpp Custom/*.h)

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
#  ${sources_Custom}
#    Custom/Transmog/Transmogrification.h
#    Custom/Transmog/Transmogrification.cpp
#    Custom/Transmog/Transmogrifier.cpp
    Custom/Boutique_ElegonWorld
    Custom/EventCustom
    Custom/Guild_System
    Custom/Fishing_tournament
    Custom/Housing_script
    Custom/Item_Teleporter
    Custom/NPC_Trainer
    Custom/script_boss_zg1
    Custom/script_cac_icw
    Custom/script_cmd_income
    Custom/script_command_debug_raid_group_maker
    Custom/script_creature_parrainage
    Custom/script_debug_rift
    Custom/script_ffa_mastodonte
    Custom/script_ks
    Custom/script_npc_LoL
    Custom/script_npc_raid
    Custom/script_rgm_decoplayer
    Custom/script_rift_creature
    Custom/script_run_rift
    Custom/scripts_sponsorship
    Custom/Transmogrification
    Custom/Transmogrifier
    Custom/Vegetable
    Custom/War_System
    Custom/ZGCustom
    Custom/Reforging.cpp 
    Custom/BeastMaster
    Custom/BountyHunter
    Custom/Lottery_system
    Custom/script_npc_info_rift
    Custom/npc_classement_loe
    Custom/npc_classement_bg
    Custom/script_remove_character
    Custom/Theramor_NPC
    Custom/script_grades
    Custom/scripts_items_campzg
    Custom/script_grade_pve
    Custom/CaptureFlags
    Custom/PresentationServer
    Custom/Premium
)

message("  -> Prepared: Custom - AMPOULE")
