////////////////////////////////////////////////////////////////////////////////
//
// Project-Hellscream https://hellscream.org
// Copyright (C) 2018-2020 Project-Hellscream-6.2
// Discord https://discord.gg/CWCF3C9
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "ulduar.h"
#include "Unit.h"
#include "Vehicle.h"
#include "GridNotifiers.h"
#include "Common.h"

#include "ace/Mutex.h"

enum Yells
{
    // Mimiron
    SAY_AGGRO                                   = 0,
    SAY_HARDMODE_ON                             = 1,
    SAY_MKII_ACTIVATE                           = 2,
    SAY_MKII_SLAY                               = 3,
    SAY_MKII_DEATH                              = 4,
    SAY_VX001_ACTIVATE                          = 5,
    SAY_VX001_SLAY                              = 6,
    SAY_VX001_DEATH                             = 7,
    SAY_AERIAL_ACTIVATE                         = 8,
    SAY_AERIAL_SLAY                             = 9,
    SAY_AERIAL_DEATH                            = 10,
    SAY_V07TRON_ACTIVATE                        = 11,
    SAY_V07TRON_SLAY                            = 12,
    SAY_V07TRON_DEATH                           = 13,
    SAY_BERSERK                                 = 14,

    // Leviathan MK II
    EMOTE_PLASMA_BLAST                          = 0
};

enum Spells
{
    SPELL_JETPACK                               = 63341, // Unused
    SPELL_EMERGENCY_MODE                        = 64582,
    SPELL_SELF_REPAIR                           = 64383,

    // Leviathan MK II
    SPELL_MINES_SPAWN                           = 65347,
    SPELL_FLAME_SUPPRESSANT_MK                  = 64570,
    SPELL_NAPALM_SHELL                          = 63666,
    SPELL_PLASMA_BLAST                          = 62997,
    SPELL_PROXIMITY_MINES                       = 63027,
    SPELL_SHOCK_BLAST                           = 63631,
    SPELL_EXPLOSION                             = 66351,
    SPELL_DEATH_GRIP                            = 49560,     // used at MK turret's immunity

    // VX 001
    SPELL_FROSTBOMB                             = 64623,    // 64627 ?
    SPELL_FROST_BOMB_VISUAL                     = 64624,
    SPELL_FROST_BOMB_EXPLOSION_10               = 64626,
    SPELL_FROST_BOMB_EXPLOSION_25               = 65333,

    SPELL_P3WX2_LASER_BARRAGE                   = 63293,    // Cast through SPELL__spinning_UP
    SPELL_RAPID_BURST                           = 63382,
    SPELL_RAPID_BURST_LEFT_10                   = 63387,
    SPELL_RAPID_BURST_RIGHT_10                  = 64019,
    SPELL_RAPID_BURST_LEFT_25                   = 64531,
    SPELL_RAPID_BURST_RIGHT_25                  = 64532,
    SPELL_ROCKET_STRIKE                         = 63036,
    SPELL_ROCKET_STRIKE_AURA                    = 64064,
    SPELL_ROCKET_STRIKE_DMG                     = 63041,
    SPELL__spinning_UP                           = 63414,
    SPELL_HEAT_WAVE                             = 63677,
    SPELL_HAND_PULSE                            = 64348,
    SPELL_SELF_STUN                             = 14821,

    // Aerial Command Unit
    SPELL_PLASMA_BALL                           = 63689,
    SPELL_MAGNETIC_CORE                         = 64436,
    SPELL_MAGNETIC_CORE_VISUAL                  = 64438,
    SPELL_BOOM_BOT                              = 63767,
    SPELL_BOOM_BOT_PERIODIC                     = 63801,
    SPELL_MAGNETIC_FIELD                        = 64668,
    SPELL_HOVER                                 = 57764, // Set Hover position
    SPELL_BERSERK                               = 47008,

    // Additonal spells
    SPELL_DEAFENING_SIREN                       = 64616,

    SPELL_SELF_DESTRUCTION                      = 64610,
    SPELL_SELF_DESTRUCTION_VISUAL               = 64613,
    SPELL_FLAME_SUPPRESSANT_VX001               = 65192,
    SPELL_SUMMON_FLAMES_INITIAL                 = 64563,
    SPELL_FLAME                                 = 64561,
    SPELL_NOT_SO_FRIENDLY_FIRE                  = 65040,

    SPELL_WATER_SPRAY                           = 64619,
    SPELL_RIDE_VEHICLE                          = 46598
};

enum Events
{
    // Mimiron
    EVENT_CHECK_TARGET              = 1,
    EVENT_ENRAGE                    = 2,
    EVENT_FLAME                     = 3,
    EVENT_STEP_1                    = 4,
    EVENT_STEP_2                    = 5,
    EVENT_STEP_3                    = 6,
    EVENT_STEP_4                    = 7,
    EVENT_STEP_5                    = 8,
    EVENT_STEP_6                    = 9,
    EVENT_STEP_7                    = 10,
    EVENT_STEP_8                    = 11,
    EVENT_STEP_9                    = 12,
    EVENT_CHECK_BOTALIVE            = 13,

    // Leviathan MK II
    EVENT_PROXIMITY_MINE            = 14,
    EVENT_NAPALM_SHELL              = 15,
    EVENT_PLASMA_BLAST              = 16,
    EVENT_SHOCK_BLAST               = 17,
    EVENT_FLAME_SUPPRESSANT         = 18,

    // VX-001
    EVENT_RAPID_BURST               = 19,
    EVENT_LASER_BARRAGE             = 20,
    EVENT_LASER_BARRAGE_END         = 21,
    EVENT_ROCKET_STRIKE             = 22,
    EVENT_HEAT_WAVE                 = 23,
    EVENT_HAND_PULSE                = 24,
    EVENT_FROST_BOMB                = 25,
    EVENT_FLAME_SUPPRESSANT_VX001   = 26,

    // Aerial unit
    EVENT_PLASMA_BALL               = 27,
    EVENT_REACTIVATE_AERIAL         = 28,
    EVENT_SUMMON_JUNK_BOT           = 29,
    EVENT_SUMMON_ASSAULT_BOT        = 30,
    EVENT_SUMMON_BOOM_BOT           = 31
};

enum Phases
{
    // Mimiron
    PHASE_IDLE                              = 0,
    PHASE_INTRO                             = 1,
    PHASE_COMBAT                            = 2,
    PHASE_VX001_ACTIVATION                  = 3,
    PHASE_AERIAL_ACTIVATION                 = 4,
    PHASE_V0L7R0N_ACTIVATION                = 5,

    // Leviathan MK II
    PHASE_LEVIATHAN_SOLO__GLOBAL_1          = 6,
    PHASE_LEVIATHAN_ASSEMBLED__GLOBAL_4     = 7,

    // VX-001
    PHASE_VX001_SOLO__GLOBAL_2              = 8,
    PHASE_VX001_ASSEMBLED__GLOBAL_4         = 9,

    // Aerial unit
    PHASE_AERIAL_SOLO__GLOBAL_3             = 10,
    PHASE_AERIAL_ASSEMBLED__GLOBAL_4        = 11
};

enum Actions
{
    DO_START_ENCOUNTER                          = 1,
    DO_ACTIVATE_VX001                           = 2,
    DO_START_VX001                              = 3,
    DO_ACTIVATE_AERIAL                          = 4,
    DO_START_AERIAL                             = 5,
    DO_DISABLE_AERIAL                           = 6,
    DO_ACTIVATE_V0L7R0N                         = 7,
    DO_LEVIATHAN_ASSEMBLED                      = 8,
    DO_LEVIATHAN_SELF_REPAIR_START              = 9,
    DO_LEVIATHAN_SELF_REPAIR_END                = 10,
    DO_VX001_ASSEMBLED                          = 11,
    DO_VX001_SELF_REPAIR_START                  = 12,
    DO_VX001_SELF_REPAIR_END                    = 13,
    DO_AERIAL_ASSEMBLED                         = 14,
    DO_AERIAL_SELF_REPAIR_START                 = 15,
    DO_AERIAL_SELF_REPAIR_END                   = 16,
    DO_ENTER_ENRAGE                             = 17,
    DO_ACTIVATE_HARD_MODE                       = 18,
    DO_INCREASE_FLAME_COUNT                     = 19,
    DO_DECREASE_FLAME_COUNT                     = 20,
    DATA_GET_HARD_MODE                          = 21,
    DATA_FLAME_COUNT                            = 22
};

enum Npcs
{
    NPC_ROCKET                                  = 34050,
    NPC_BURST_TARGET                            = 34211,
    NPC_JUNK_BOT                                = 33855,
    NPC_ASSAULT_BOT                             = 34057,
    NPC_BOOM_BOT                                = 33836,
    NPC_EMERGENCY_BOT                           = 34147,
    NPC_FLAMES_INITIAL                          = 34363,
    NPC_FLAME_SPREAD                            = 34121,
    NPC_FROST_BOMB                              = 34149,
    NPC_MKII_TURRET                             = 34071,
    NPC_PROXIMITY_MINE                          = 34362,
    NPC_MIMIRON_FOCUS                           = 33835
};

// Achievements
#define ACHIEVEMENT_SET_UP_US_THE_BOMB          RAID_MODE(2989, 3237)
// TODO:
// Achiev 2989: Criterias 10543, 10544, 10545
// Achiev 3237: Criterias 10546, 10547, 10548
// Which criteria is reponsible for which step ?
// 26491 - 38080 - 45033 <- what do these numbers mean ?

enum BombIndices // Scripts are present, but criteria-id <-> script is missing
{
    DATA_AVOIDED_PROXIMITY_MINES     = 30,
    DATA_AVOIDED_ROCKET_STRIKES,
    DATA_AVOIDED_BOOM_BOT_EXPLOSION
};

enum MimironChests
{
    CACHE_OF_INNOVATION_10                      = 194789,
    CACHE_OF_INNOVATION_HARDMODE_10             = 194957,
    CACHE_OF_INNOVATION_25                      = 194956,
    CACHE_OF_INNOVATION_HARDMODE_25             = 194958
};

Position const SummonPos[9] =
{
    {2703.93f, 2569.32f, 364.397f, 0},
    {2715.33f, 2569.23f, 364.397f, 0},
    {2726.85f, 2569.28f, 364.397f, 0},
    {2765.24f, 2534.38f, 364.397f, 0},
    {2759.54f, 2544.30f, 364.397f, 0},
    {2753.82f, 2554.22f, 364.397f, 0},
    {2764.95f, 2604.11f, 364.397f, 0},
    {2759.19f, 2594.26f, 364.397f, 0},
    {2753.56f, 2584.30f, 364.397f, 0}
};

#define FLAME_CAP 200 // find a good (blizzlike!) value

struct EqualHelper // Helper for Mimirons map that stores the members repair states
{
    EqualHelper(bool& base) : __base(base) {}
    void operator()(std::pair<uint32, bool> value)
    {
        __base = __base && value.second;
    }

private:
    bool& __base;
};

class boss_mimiron : public CreatureScript
{
    public:
        boss_mimiron() : CreatureScript("boss_mimiron") {}

        struct boss_mimironAI : public BossAI
        {
            boss_mimironAI(Creature* creature) : BossAI(creature, BOSS_MIMIRON)
            {
                me->ApplySpellImmune(0, IMMUNITY_ID, SPELL_ROCKET_STRIKE_DMG, true);
                me->SetReactState(REACT_PASSIVE);
                _gotEncounterFinished = false;
            }

            void DespawnCreatures(uint32 entry, float distance)
            {
                std::list<Creature*> creatures;
                GetCreatureListWithEntryInGrid(creatures, me, entry, distance);

                if (creatures.empty())
                    return;

                for (std::list<Creature*>::iterator iter = creatures.begin(); iter != creatures.end(); ++iter)
                    (*iter)->DespawnOrUnsummon();
            }

            void Reset()
            {
                _Reset();
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_USE_STANDING);
                me->SetVisible(true);
                me->ExitVehicle();
                me->GetMotionMaster()->MoveTargetedHome();

                instance->SetData(DATA_MIMIRON_ELEVATOR, GO_STATE_ACTIVE);

                _phase = PHASE_IDLE;
                events.SetPhase(_phase);
                _flameCount = 0;
                _gotHardMode = false;
                _enraged = false;

                //events.ScheduleEvent(EVENT_CHECK_TARGET, 7*IN_MILLISECONDS);

                DespawnCreatures(NPC_FLAMES_INITIAL, 100.0f);
                DespawnCreatures(NPC_PROXIMITY_MINE, 100.0f);
                DespawnCreatures(NPC_ROCKET, 100.0f);
                DespawnCreatures(NPC_JUNK_BOT, 100.0f);
                DespawnCreatures(NPC_ASSAULT_BOT, 100.0f);
                DespawnCreatures(NPC_BOOM_BOT, 100.0f);
                DespawnCreatures(NPC_EMERGENCY_BOT, 100.0f);

                for (uint8 data = DATA_LEVIATHAN_MK_II; data <= DATA_AERIAL_UNIT; ++data)
                    if (Creature* creature = ObjectAccessor::GetCreature(*me, instance->GetData64(data)))
                        if (creature->isAlive())
                        {
                            creature->ExitVehicle();
                            creature->AI()->EnterEvadeMode();
                        }

                if (GameObject* go = me->FindNearestGameObject(GO_BIG_RED_BUTTON, 200))
                {
                    go->SetGoState(GO_STATE_READY);
                    go->SetLootState(GO_JUST_DEACTIVATED);
                    go->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
                }

                _isSelfRepairing[DATA_LEVIATHAN_MK_II] = false;
                _isSelfRepairing[DATA_VX_001] = false;
                _isSelfRepairing[DATA_AERIAL_UNIT] = false;

                _setUpUsTheBomb[DATA_AVOIDED_PROXIMITY_MINES] = true;
                _setUpUsTheBomb[DATA_AVOIDED_ROCKET_STRIKES] = true;
                _setUpUsTheBomb[DATA_AVOIDED_BOOM_BOT_EXPLOSION] = true;
            }

            void BotAliveCheck()
            {
                _mapMutex.acquire();
                bool res = true;
                // Check if there is still a false value.
                std::for_each(_isSelfRepairing.begin(), _isSelfRepairing.end(), EqualHelper(res));
                _mapMutex.release();
                if (res)
                {
                    // We're down, baby.
                    Creature* Leviathan = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_LEVIATHAN_MK_II));
                    Creature* VX_001 = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_VX_001));
                    Creature* AerialUnit = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_AERIAL_UNIT));
                    if (Leviathan && VX_001 && AerialUnit)
                    {
                        Leviathan->DisappearAndDie();
                        VX_001->DisappearAndDie();
                        AerialUnit->DisappearAndDie();
                        DespawnCreatures(NPC_FLAMES_INITIAL, 100.0f);
                        DespawnCreatures(NPC_PROXIMITY_MINE, 100.0f);
                        DespawnCreatures(NPC_ROCKET, 100);
                        me->ExitVehicle();
                        EncounterPostProgress();
                    }
                }
            }

            void EncounterPostProgress()
            {
                Talk(SAY_V07TRON_DEATH);

                me->SetReactState(REACT_PASSIVE);
                me->RemoveAllAuras();
                me->AttackStop();
                me->setFaction(35);
                me->DeleteThreatList();
                me->CombatStop(true);
                if (instance)
                {
                     if (_gotHardMode)
                        me->SummonGameObject(RAID_MODE(CACHE_OF_INNOVATION_HARDMODE_10, CACHE_OF_INNOVATION_HARDMODE_25), 2744.65f, 2569.46f, 364.314f, 3.14159f, 0, 0, 0.7f, 0.7f, WEEK);
                    else
                        me->SummonGameObject(RAID_MODE(CACHE_OF_INNOVATION_10, CACHE_OF_INNOVATION_25), 2744.65f, 2569.46f, 364.314f, 3.14159f, 0, 0, 0.7f, 0.7f, WEEK);

                    instance->DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE, NPC_LEVIATHAN_MKII, 1);
                }
                _JustDied();
                me->DespawnOrUnsummon(5*IN_MILLISECONDS);
            }

            void EnterCombat(Unit* /*who*/)
            {
                _EnterCombat();
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1);
                _phase = PHASE_INTRO;
                events.SetPhase(_phase);
                if (_gotHardMode)
                    events.ScheduleEvent(EVENT_FLAME, 5*IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_ENRAGE, _gotHardMode ? 10*MINUTE*IN_MILLISECONDS : 15*MINUTE*IN_MILLISECONDS); // Enrage in 10 (hard mode) or 15 min
                events.ScheduleEvent(EVENT_STEP_1, 0.1*IN_MILLISECONDS, 0, _phase);

                if (GameObject* go = me->FindNearestGameObject(GO_BIG_RED_BUTTON, 200.0f))
                    go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
            }

            void JustDied(Unit* /*victim*/)
            {
                // for testing purpose
                EncounterPostProgress();
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CHECK_TARGET:
                            // prevent mimiron staying infight with leviathan introduced in rev #b40bf69
                            // TODO: find out why this happens
                            if (!SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, true))
                            {
                                EnterEvadeMode();
                                return;
                            }
                            events.ScheduleEvent(EVENT_CHECK_TARGET, 7*IN_MILLISECONDS);
                            return;
                        case EVENT_ENRAGE:
                            Talk(SAY_BERSERK);
                            for (uint8 data = DATA_LEVIATHAN_MK_II; data <= DATA_AERIAL_UNIT; ++data)
                                if (Creature* creature = ObjectAccessor::GetCreature(*me, instance->GetData64(data)))
                                    creature->AI()->DoAction(DO_ENTER_ENRAGE);
                            _enraged = true;
                            if (_gotHardMode)
                            {
                                DoCast(me, SPELL_SELF_DESTRUCTION, true);
                                DoCast(me, SPELL_SELF_DESTRUCTION_VISUAL, true);
                            }
                            return;
                        case EVENT_FLAME:
                            for (uint8 i = 0; i < 3; ++i)
                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                    DoCast(target, SPELL_SUMMON_FLAMES_INITIAL, true);
                            events.ScheduleEvent(EVENT_FLAME, 30*IN_MILLISECONDS);
                            return;
                        case EVENT_STEP_1:
                            switch (_phase)
                            {
                                case PHASE_INTRO:
                                    Talk(_gotHardMode ? SAY_HARDMODE_ON : SAY_AGGRO);
                                    events.ScheduleEvent(EVENT_STEP_2, 10*IN_MILLISECONDS, 0, PHASE_INTRO);
                                    break;
                                case PHASE_VX001_ACTIVATION:
                                    Talk(SAY_MKII_DEATH);
                                    events.ScheduleEvent(EVENT_STEP_2, 10*IN_MILLISECONDS, 0, PHASE_VX001_ACTIVATION);
                                    break;
                                case PHASE_AERIAL_ACTIVATION:
                                    me->ChangeSeat(4);
                                    me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_STAND);
                                    events.ScheduleEvent(EVENT_STEP_2, 2.5*IN_MILLISECONDS, 0, PHASE_AERIAL_ACTIVATION);
                                    break;
                                case PHASE_V0L7R0N_ACTIVATION:
                                    if (instance)
                                    {
                                        //me->SetVisible(true);
                                        if (Creature* Leviathan = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_LEVIATHAN_MK_II)))
                                            Leviathan->GetMotionMaster()->MovePoint(0, 2744.65f, 2569.46f, 364.397f);
                                        if (Creature* VX_001 = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_VX_001)))
                                        {
                                            me->EnterVehicle(VX_001, 1);
                                            me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_STAND);
                                            Talk(SAY_AERIAL_DEATH);
                                        }
                                    }
                                    events.ScheduleEvent(EVENT_STEP_2, 5*IN_MILLISECONDS, 0, PHASE_V0L7R0N_ACTIVATION);
                                    break;
                                default:
                                    break;
                            }
                            return;
                        case EVENT_STEP_2:
                            switch (_phase)
                            {
                                case PHASE_INTRO:
                                    if (instance)
                                    {
                                        if (Creature* Leviathan = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_LEVIATHAN_MK_II)))
                                            me->EnterVehicle(Leviathan, 4);
                                    }
                                    events.ScheduleEvent(EVENT_STEP_3, 2*IN_MILLISECONDS, 0, PHASE_INTRO);
                                    break;
                                case PHASE_VX001_ACTIVATION:
                                    me->ChangeSeat(1);
                                    events.ScheduleEvent(EVENT_STEP_3, 2*IN_MILLISECONDS, 0, PHASE_VX001_ACTIVATION);
                                    break;
                                case PHASE_AERIAL_ACTIVATION:
                                    Talk(SAY_VX001_DEATH);
                                    me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_TALK);
                                    events.ScheduleEvent(EVENT_STEP_3, 5*IN_MILLISECONDS, 0, PHASE_AERIAL_ACTIVATION);
                                    break;
                                case PHASE_V0L7R0N_ACTIVATION:
                                    if (instance)
                                        if (Creature* VX_001 = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_VX_001)))
                                            if (Creature* Leviathan = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_LEVIATHAN_MK_II)))
                                            {
                                                VX_001->SetStandState(UNIT_STAND_STATE_STAND);
                                                VX_001->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_CUSTOM_SPELL_01);
                                                VX_001->EnterVehicle(Leviathan, 7);
                                            }
                                    events.ScheduleEvent(EVENT_STEP_3, 2*IN_MILLISECONDS, 0, PHASE_V0L7R0N_ACTIVATION);
                                    break;
                                default:
                                    break;
                            }
                            return;
                        case EVENT_STEP_3:
                            switch (_phase)
                            {
                                case PHASE_INTRO:
                                    me->ChangeSeat(2);
                                    events.ScheduleEvent(EVENT_STEP_4, 2*IN_MILLISECONDS, 0, PHASE_INTRO);
                                    break;
                                case PHASE_VX001_ACTIVATION:
                                    if (instance)
                                        instance->SetData(DATA_MIMIRON_ELEVATOR, GO_STATE_READY);
                                    events.ScheduleEvent(EVENT_STEP_4, 15*IN_MILLISECONDS, 0, PHASE_VX001_ACTIVATION);
                                    break;
                                case PHASE_AERIAL_ACTIVATION:
                                    me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_STAND);
                                    if (instance)
                                    {
                                        if (Creature* AerialUnit = me->SummonCreature(NPC_AERIAL_COMMAND_UNIT, 2744.65f, 2569.46f, 380.0f, 3.14159f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10*IN_MILLISECONDS))
                                        {
                                            AerialUnit->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                            AerialUnit->SetVisible(true);
                                        }
                                    }
                                    events.ScheduleEvent(EVENT_STEP_4, 5*IN_MILLISECONDS, 0, PHASE_AERIAL_ACTIVATION);
                                    break;
                                case PHASE_V0L7R0N_ACTIVATION:
                                    if (instance)
                                        if (Creature* VX_001 = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_VX_001)))
                                            if (Creature* AerialUnit = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_AERIAL_UNIT)))
                                            {
                                                AerialUnit->SetCanFly(false);
                                                AerialUnit->EnterVehicle(VX_001, 3);
                                                Talk(SAY_V07TRON_ACTIVATE);
                                            }
                                    events.ScheduleEvent(EVENT_STEP_4, 10*IN_MILLISECONDS, 0, PHASE_V0L7R0N_ACTIVATION);
                                    break;
                                default:
                                    break;
                            }
                            return;
                        case EVENT_STEP_4:
                            switch (_phase)
                            {
                                case PHASE_INTRO:
                                    me->ChangeSeat(5);
                                    me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_STAND);
                                    events.ScheduleEvent(EVENT_STEP_5, 2.5*IN_MILLISECONDS, 0, PHASE_INTRO);
                                    break;
                                case PHASE_VX001_ACTIVATION:
                                    if (instance)
                                    {
                                        if (Creature* VX_001 = me->SummonCreature(NPC_VX_001, 2744.65f, 2569.46f, 364.397f, 3.14159f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10*IN_MILLISECONDS))
                                        {
                                            instance->SetData(DATA_MIMIRON_ELEVATOR, GO_STATE_ACTIVE_ALTERNATIVE);
                                            VX_001->SetVisible(true);
                                            VX_001->setFaction(35);
                                            VX_001->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                                            for (uint8 n = 5; n < 7; ++n)
                                            {
                                                if (Creature* Rocket = VX_001->SummonCreature(NPC_ROCKET, VX_001->GetPositionX(), VX_001->GetPositionY(), VX_001->GetPositionZ(), 0, TEMPSUMMON_MANUAL_DESPAWN))
                                                {
                                                    Rocket->setFaction(14);
                                                    Rocket->SetReactState(REACT_PASSIVE);
                                                    Rocket->EnterVehicle(VX_001, n);
                                                }
                                            }
                                        }
                                    }
                                    events.ScheduleEvent(EVENT_STEP_5, 8*IN_MILLISECONDS, 0, PHASE_VX001_ACTIVATION);
                                    break;
                                case PHASE_AERIAL_ACTIVATION:
                                    me->ExitVehicle();
                                    //me->GetMotionMaster()->MoveJump(2745.06f, 2569.36f, 379.90f, 10, 15);
                                    // entering the vehicle makes Mimiron leave combat and restart the encounter
                                    //if (Creature* AerialUnit = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_AERIAL_UNIT)))
                                      //  me->EnterVehicle(AerialUnit, 0);
                                    events.ScheduleEvent(EVENT_STEP_5, 2*IN_MILLISECONDS, 0, PHASE_AERIAL_ACTIVATION);
                                    break;
                                case PHASE_V0L7R0N_ACTIVATION:
                                    if (instance)
                                    {
                                        if (Creature* Leviathan = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_LEVIATHAN_MK_II)))
                                            Leviathan->AI()->DoAction(DO_LEVIATHAN_ASSEMBLED);
                                        if (Creature* VX_001 = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_VX_001)))
                                            VX_001->AI()->DoAction(DO_VX001_ASSEMBLED);
                                        if (Creature* AerialUnit = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_AERIAL_UNIT)))
                                            AerialUnit->AI()->DoAction(DO_AERIAL_ASSEMBLED);
                                        _phase = PHASE_COMBAT;
                                        events.SetPhase(_phase);
                                    }
                                    break;
                                default:
                                    break;
                            }
                            return;
                        case EVENT_STEP_5:
                            switch (_phase)
                            {
                                case PHASE_INTRO:
                                    Talk(SAY_MKII_ACTIVATE);
                                    me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_TALK);
                                    events.ScheduleEvent(EVENT_STEP_6, 6*IN_MILLISECONDS, 0, PHASE_INTRO);
                                    break;
                                case PHASE_VX001_ACTIVATION:
                                    if (instance)
                                        if (Creature* VX_001 = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_VX_001)))
                                            me->EnterVehicle(VX_001, 0);
                                    events.ScheduleEvent(EVENT_STEP_6, 3.5*IN_MILLISECONDS, 0, PHASE_VX001_ACTIVATION);
                                    break;
                                case PHASE_AERIAL_ACTIVATION:
                                    me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_TALK);
                                    Talk(SAY_AERIAL_ACTIVATE);
                                    events.ScheduleEvent(EVENT_STEP_6, 8*IN_MILLISECONDS, 0, PHASE_AERIAL_ACTIVATION);
                                    break;
                                default:
                                    break;
                            }
                            return;
                        case EVENT_STEP_6:
                            switch (_phase)
                            {
                                case PHASE_INTRO:
                                    me->ChangeSeat(6);
                                    events.ScheduleEvent(EVENT_STEP_7, 2*IN_MILLISECONDS, 0, PHASE_INTRO);
                                    break;
                                case PHASE_VX001_ACTIVATION:
                                    me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_TALK);
                                    Talk(SAY_VX001_ACTIVATE);
                                    events.ScheduleEvent(EVENT_STEP_7, 10*IN_MILLISECONDS, 0, PHASE_VX001_ACTIVATION);
                                    break;
                                case PHASE_AERIAL_ACTIVATION:
                                    me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_STAND);
                                    //me->SetVisible(false);
                                    if (instance)
                                    {
                                        if (Creature* AerialUnit = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_AERIAL_UNIT)))
                                        {
                                            AerialUnit->AI()->DoAction(DO_START_AERIAL);
                                            AerialUnit->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                            _phase = PHASE_COMBAT;
                                            events.SetPhase(_phase);
                                        }
                                    }
                                    break;
                                default:
                                    break;
                            }
                            return;
                        case EVENT_STEP_7:
                            switch (_phase)
                            {
                                case PHASE_INTRO:
                                    if (instance)
                                    {
                                        if (Creature* Leviathan = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_LEVIATHAN_MK_II)))
                                        {
                                            me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_STAND);
                                            Leviathan->AI()->DoAction(DO_START_ENCOUNTER);
                                            _phase = PHASE_COMBAT;
                                            events.SetPhase(_phase);
                                        }
                                    }
                                    break;
                                case PHASE_VX001_ACTIVATION:
                                    me->ChangeSeat(1);
                                    me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_SIT);
                                    events.ScheduleEvent(EVENT_STEP_8, 2*IN_MILLISECONDS, 0, PHASE_VX001_ACTIVATION);
                                    break;
                                default:
                                    break;
                            }
                            return;
                        case EVENT_STEP_8:
                            switch (_phase)
                            {
                                case PHASE_VX001_ACTIVATION:
                                    if (instance)
                                    {
                                        if (Creature* VX_001 = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_VX_001)))
                                            VX_001->HandleEmoteCommand(EMOTE_ONESHOT_EMERGE);
                                    }
                                    events.ScheduleEvent(EVENT_STEP_9, 3.5*IN_MILLISECONDS, 0, PHASE_VX001_ACTIVATION);
                                    break;
                                default:
                                    break;
                            }
                            return;
                        case EVENT_STEP_9:
                            switch (_phase)
                            {
                                case PHASE_VX001_ACTIVATION:
                                    if (instance)
                                        if (Creature* VX_001 = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_VX_001)))
                                        {
                                            VX_001->AddAura(SPELL_HOVER, VX_001); // Hover
                                            VX_001->AI()->DoAction(DO_START_VX001);
                                            _phase = PHASE_COMBAT;
                                            events.SetPhase(_phase);
                                        }
                                    break;
                                default:
                                    break;
                            }
                            return;
                        default:
                            return;
                    }
                }
            }

            uint32 GetData(uint32 type)
            {
                switch (type)
                {
                    case DATA_GET_HARD_MODE:
                        return _gotHardMode ? 1 : 0;
                    case DATA_FLAME_COUNT:
                        return _flameCount;
                    case DATA_AVOIDED_ROCKET_STRIKES:
                        return _setUpUsTheBomb[DATA_AVOIDED_ROCKET_STRIKES] ? 1 : 0;
                    case DATA_AVOIDED_PROXIMITY_MINES:
                        return _setUpUsTheBomb[DATA_AVOIDED_PROXIMITY_MINES] ? 1 : 0;
                    case DATA_AVOIDED_BOOM_BOT_EXPLOSION:
                        return _setUpUsTheBomb[DATA_AVOIDED_BOOM_BOT_EXPLOSION] ? 1 : 0;
                    default:
                        break;
                }
                return 0;
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    case DO_ACTIVATE_VX001:
                        _phase = PHASE_VX001_ACTIVATION;
                        events.SetPhase(PHASE_VX001_ACTIVATION);
                        events.ScheduleEvent(EVENT_STEP_1, 0.1*IN_MILLISECONDS, 0, PHASE_VX001_ACTIVATION);
                        break;
                    case DO_ACTIVATE_AERIAL:
                        _phase = PHASE_AERIAL_ACTIVATION;
                        events.SetPhase(PHASE_AERIAL_ACTIVATION);
                        events.ScheduleEvent(EVENT_STEP_1, 5*IN_MILLISECONDS, 0, PHASE_AERIAL_ACTIVATION);
                        break;
                    case DO_ACTIVATE_V0L7R0N:
                        me->SetVisible(true);
                        _phase = PHASE_V0L7R0N_ACTIVATION;
                        events.SetPhase(PHASE_V0L7R0N_ACTIVATION);
                        events.ScheduleEvent(EVENT_STEP_1, 1*IN_MILLISECONDS, 0, PHASE_V0L7R0N_ACTIVATION);
                        break;
                    case DO_ACTIVATE_HARD_MODE:     // Cannot be done infight, since the button gets locked on EnterCombat() with Mimiron.
                        _gotHardMode = true;
                        DoZoneInCombat();
                        break;
                    case DO_INCREASE_FLAME_COUNT:
                        ++_flameCount;
                        break;
                    case DO_DECREASE_FLAME_COUNT:
                        if (_flameCount)
                            --_flameCount;
                        break;
                    // Repair stuff
                    case DO_LEVIATHAN_SELF_REPAIR_START:
                        _mapMutex.acquire();
                        _isSelfRepairing[DATA_LEVIATHAN_MK_II] = true;
                        _mapMutex.release();
                        BotAliveCheck();
                        break;
                    case DO_LEVIATHAN_SELF_REPAIR_END:
                        _mapMutex.acquire();
                        _isSelfRepairing[DATA_LEVIATHAN_MK_II] = false;
                        _mapMutex.release();
                        break;
                    case DO_VX001_SELF_REPAIR_START:
                        _mapMutex.acquire();
                        _isSelfRepairing[DATA_VX_001] = true;
                        _mapMutex.release();
                        BotAliveCheck();
                        break;
                    case DO_VX001_SELF_REPAIR_END:
                        _mapMutex.acquire();
                        _isSelfRepairing[DATA_VX_001] = false;
                        _mapMutex.release();
                        break;
                    case DO_AERIAL_SELF_REPAIR_START:
                        _mapMutex.acquire();
                        _isSelfRepairing[DATA_AERIAL_UNIT] = true;
                        _mapMutex.release();
                        BotAliveCheck();
                        break;
                    case DO_AERIAL_SELF_REPAIR_END:
                        _mapMutex.acquire();
                        _isSelfRepairing[DATA_AERIAL_UNIT] = false;
                        _mapMutex.release();
                        break;
                    // Achiev
                    case DATA_AVOIDED_ROCKET_STRIKES:
                        _setUpUsTheBomb[DATA_AVOIDED_ROCKET_STRIKES] = false;
                        break;
                    case DATA_AVOIDED_PROXIMITY_MINES:
                        _setUpUsTheBomb[DATA_AVOIDED_PROXIMITY_MINES] = false;
                        break;
                    case DATA_AVOIDED_BOOM_BOT_EXPLOSION:
                        _setUpUsTheBomb[DATA_AVOIDED_BOOM_BOT_EXPLOSION] = false;
                        break;
                    default:
                        break;
                }
            }

            private:
                ACE_Mutex _mapMutex;
                std::map<uint32, bool> _isSelfRepairing;
                std::map<BombIndices, bool> _setUpUsTheBomb;
                Phases _phase;
                uint32 _flameCount;
                bool _gotHardMode;
                bool _enraged;
                bool _gotEncounterFinished;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetUlduarAI<boss_mimironAI>(creature);
        }
};

class boss_leviathan_mk : public CreatureScript
{
    public:
        boss_leviathan_mk() : CreatureScript("boss_leviathan_mk") {}

        struct boss_leviathan_mkAI : public ScriptedAI
        {
            boss_leviathan_mkAI(Creature* creature) : ScriptedAI(creature) {}

            void InitializeAI()
            {
                _instance = me->GetInstanceScript();
                me->ApplySpellImmune(0, IMMUNITY_ID, SPELL_ROCKET_STRIKE_DMG, true);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                Reset();
            }

            void Reset()
            {
                _events.Reset();
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1);
                me->SetStandState(UNIT_STAND_STATE_STAND);
                me->SetReactState(REACT_PASSIVE);
                me->RemoveAllAurasExceptType(SPELL_AURA_CONTROL_VEHICLE);
                _phase = PHASE_IDLE;
                _events.SetPhase(_phase);
                _gotMimironHardMode = false;

                if (Creature* turret = CAST_CRE(me->GetVehicleKit()->GetPassenger(3)))
                {
                    turret->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1);
                    turret->SetReactState(REACT_PASSIVE);
                }
            }

            void KilledUnit(Unit* /*who*/)
            {
                if (uint64 Mimiron = _instance->GetData64(BOSS_MIMIRON))
                {
                    if (_phase == PHASE_LEVIATHAN_SOLO__GLOBAL_1)
                    {
                        if (!urand(0,5))
                            Talk(SAY_MKII_SLAY, Mimiron);
                    }
                    else
                        if (!urand(0,5))
                            Talk(SAY_V07TRON_SLAY, Mimiron);
                }
            }

            void SpellHit(Unit* caster, SpellInfo const* spell)
            {
                if (!caster || !spell)
                    return;

                if (spell->Id == SPELL_SELF_REPAIR)
                {
                    DoAction(DO_LEVIATHAN_SELF_REPAIR_END);
                    if (Creature* Mimiron = ObjectAccessor::GetCreature(*me, _instance->GetData64(BOSS_MIMIRON)))
                        Mimiron->AI()->DoAction(DO_LEVIATHAN_SELF_REPAIR_END);
                }
            }

            void DamageTaken(Unit* /*who*/, uint32 &damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (damage >= me->GetHealth())
                {
                    // Common stuff
                    damage = 0;
                    me->InterruptNonMeleeSpells(true);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1);
                    me->AttackStop();
                    me->SetReactState(REACT_PASSIVE);
                    me->RemoveAllAurasExceptType(SPELL_AURA_CONTROL_VEHICLE);
                    switch (_phase)
                    {
                        case PHASE_LEVIATHAN_SOLO__GLOBAL_1:
                            me->SetHealth(me->GetMaxHealth());
                            if (Creature* Mimiron = ObjectAccessor::GetCreature(*me, _instance->GetData64(BOSS_MIMIRON)))
                                Mimiron->AI()->DoAction(DO_ACTIVATE_VX001);
                            if (Creature* turret = CAST_CRE(me->GetVehicleKit()->GetPassenger(3)))
                                turret->Kill(turret, false);
                            me->SetSpeed(MOVE_RUN, 1.5f, true);
                            me->GetMotionMaster()->MovePoint(0, 2790.11f, 2595.83f, 364.32f);
                            break;
                        case PHASE_LEVIATHAN_ASSEMBLED__GLOBAL_4:
                            me->SetStandState(UNIT_STAND_STATE_DEAD);
                            if (Creature* Mimiron = ObjectAccessor::GetCreature(*me, _instance->GetData64(BOSS_MIMIRON)))
                                Mimiron->AI()->DoAction(DO_LEVIATHAN_SELF_REPAIR_START);
                            DoCast(me, SPELL_SELF_REPAIR);
                            break;
                        default:
                            break;
                    }
                    _events.Reset(); // Wipe events, just for the case
                    _phase = PHASE_IDLE;
                    _events.SetPhase(_phase);
                }
            }

            void EnterCombat(Unit* /*who*/)
            {
                if (Creature* Mimiron = ObjectAccessor::GetCreature(*me, _instance ? _instance->GetData64(BOSS_MIMIRON) : 0))
                    _gotMimironHardMode = Mimiron->AI()->GetData(DATA_GET_HARD_MODE);

                if (Creature* turret = CAST_CRE(me->GetVehicleKit()->GetPassenger(3)))
                {
                    turret->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1);
                    turret->SetReactState(REACT_AGGRESSIVE);
                    turret->AI()->DoZoneInCombat();
                }

                _events.ScheduleEvent(EVENT_SHOCK_BLAST, 30*IN_MILLISECONDS, 0, PHASE_LEVIATHAN_SOLO__GLOBAL_1);
                _events.ScheduleEvent(EVENT_PROXIMITY_MINE, 0, 0, PHASE_LEVIATHAN_SOLO__GLOBAL_1);
                _events.ScheduleEvent(EVENT_PLASMA_BLAST, 10*IN_MILLISECONDS, 0, PHASE_LEVIATHAN_SOLO__GLOBAL_1);
                if (_gotMimironHardMode)
                {
                    DoCast(me, SPELL_EMERGENCY_MODE, true);
                    _events.ScheduleEvent(EVENT_FLAME_SUPPRESSANT, 1*MINUTE*IN_MILLISECONDS, 0, PHASE_LEVIATHAN_SOLO__GLOBAL_1);
                }
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    case DO_START_ENCOUNTER:
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1 | UNIT_FLAG_IMMUNE_TO_PC);
                        me->SetReactState(REACT_AGGRESSIVE);
                        _phase = PHASE_LEVIATHAN_SOLO__GLOBAL_1;
                        _events.SetPhase(_phase);
                        DoZoneInCombat();
                        break;
                    case DO_LEVIATHAN_ASSEMBLED:                            // Assemble and self-repair share some stuff, so the fallthrough is intended!
                        me->SetHealth( (me->GetMaxHealth() >> 1) );
                        // no break here
                    case DO_LEVIATHAN_SELF_REPAIR_END:
                        if (_gotMimironHardMode)
                            if (!me->HasAura(SPELL_EMERGENCY_MODE))
                                DoCast(me, SPELL_EMERGENCY_MODE, true);
                        _phase = PHASE_LEVIATHAN_ASSEMBLED__GLOBAL_4;
                        _events.SetPhase(_phase);
                        me->InterruptNonMeleeSpells(false);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1);
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->SetSpeed(MOVE_RUN, 1.0f, true);
                        me->SetStandState(UNIT_STAND_STATE_STAND);
                        _events.RescheduleEvent(EVENT_PROXIMITY_MINE, 1*IN_MILLISECONDS, 0, PHASE_LEVIATHAN_ASSEMBLED__GLOBAL_4);
                        _events.RescheduleEvent(EVENT_SHOCK_BLAST, 3*IN_MILLISECONDS, 0, PHASE_LEVIATHAN_ASSEMBLED__GLOBAL_4);
                        break;
                    case DO_ENTER_ENRAGE:
                        DoCast(me, SPELL_BERSERK, true);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim() || _phase == PHASE_IDLE)
                    return;

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING) || me->HasUnitState(UNIT_STATE_STUNNED))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_PROXIMITY_MINE:
                            {
                                DoCast(SPELL_PROXIMITY_MINES);
                                _events.RescheduleEvent(EVENT_PROXIMITY_MINE, 35*IN_MILLISECONDS, _phase);
                            }
                            return;
                        case EVENT_PLASMA_BLAST:
                            Talk(EMOTE_PLASMA_BLAST);
                            DoCast(SPELL_PLASMA_BLAST);
                            _events.RescheduleEvent(EVENT_PLASMA_BLAST, urand(30*IN_MILLISECONDS, 35*IN_MILLISECONDS), 0, _phase);
                            _events.RescheduleEvent(EVENT_SHOCK_BLAST, urand(6*IN_MILLISECONDS, 10*IN_MILLISECONDS), 0, _phase);
                            return;
                        case EVENT_SHOCK_BLAST:
                            DoCastAOE(SPELL_SHOCK_BLAST);
                            _events.RescheduleEvent(EVENT_SHOCK_BLAST, 35*IN_MILLISECONDS, 0, _phase);
                            return;
                        case EVENT_FLAME_SUPPRESSANT:
                            DoCastAOE(SPELL_FLAME_SUPPRESSANT_MK);
                            _events.RescheduleEvent(EVENT_FLAME_SUPPRESSANT, 1*MINUTE*IN_MILLISECONDS, 0, PHASE_LEVIATHAN_SOLO__GLOBAL_1);
                            return;
                        default:
                            return;
                    }
                }

                DoMeleeAttackIfReady();
            }

            private:
                Phases _phase;
                EventMap _events;
                bool _gotMimironHardMode;
                InstanceScript* _instance;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetUlduarAI<boss_leviathan_mkAI>(creature);
        }
};

// Levi's turret
class boss_leviathan_mk_turret : public CreatureScript
{
    public:
        boss_leviathan_mk_turret() : CreatureScript("boss_leviathan_mk_turret") {}

        struct boss_leviathan_mk_turretAI : public ScriptedAI
        {
            boss_leviathan_mk_turretAI(Creature* c) : ScriptedAI(c) {}

            void InitializeAI()
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_ID, SPELL_DEATH_GRIP, true);
                Reset();
            }

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);
                _napalmShellTimer = urand(8*IN_MILLISECONDS, 12*IN_MILLISECONDS);
            }

            // prefer ranged targets
            Unit* GetNapalmShellTarget()
            {
                if (Map* map = me->GetMap())
                {
                    std::list<Player*> playerList;
                    Map::PlayerList const& Players = map->GetPlayers();
                    for (Map::PlayerList::const_iterator itr = Players.begin(); itr != Players.end(); ++itr)
                    {
                        if (Player* player = itr->getSource())
                        {
                            if (player->isDead() || player->isGameMaster())
                                continue;

                            float Distance = player->GetDistance(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                            if (Distance < 15.0f || Distance > 100.0f)
                                continue;

                            playerList.push_back(player);
                        }
                    }
                    if (playerList.empty())
                        return SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true);
                    else
                        return Trinity::Containers::SelectRandomContainerElement(playerList);
                }
                else
                    return 0;
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                if (_napalmShellTimer <= diff)
                {
                    if (Unit* shellTarget = GetNapalmShellTarget())
                        DoCast(shellTarget, SPELL_NAPALM_SHELL);
                    _napalmShellTimer = urand(8*IN_MILLISECONDS, 12*IN_MILLISECONDS);
                }
                else
                    _napalmShellTimer -= diff;
            }

            private:
                uint32 _napalmShellTimer;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetUlduarAI<boss_leviathan_mk_turretAI>(creature);
        }
};

// Npc that triggers the mine explosion
// Bomb will explode after 35 seconds or if a potential target is within half a meter
class npc_proximity_mine : public CreatureScript
{
    public:
        npc_proximity_mine() : CreatureScript("npc_proximity_mine") {}

        struct npc_proximity_mineAI : public Scripted_NoMovementAI
        {
            npc_proximity_mineAI(Creature* creature) : Scripted_NoMovementAI(creature) {}

            void InitializeAI()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                _boomTimer = 35*IN_MILLISECONDS;
                _boomLocked = false;
            }

            void SpellHitTarget(Unit* target, SpellInfo const* spell)
            {
                if (!target || !spell)
                    return;

                if (target->ToPlayer())
                    if (spell->Id == SPELL_EXPLOSION)
                        if (InstanceScript* instance = me->GetInstanceScript())
                            if (Creature* mimiron = ObjectAccessor::GetCreature(*me, instance->GetData64(BOSS_MIMIRON)))
                                mimiron->AI()->DoAction(DATA_AVOIDED_PROXIMITY_MINES);
            }

            void MoveInLineOfSight(Unit* who)
            {
                if (!who)
                    return;

                if (Player* player = who->ToPlayer())
                    if (!player->isGameMaster())
                        if (!_boomLocked && me->GetDistance2d(player) < 2.0f)
                        {
                            DoCastAOE(SPELL_EXPLOSION);
                            _boomLocked = true;
                            me->DespawnOrUnsummon(1*IN_MILLISECONDS);
                        }
            }

            void UpdateAI(const uint32 diff)
            {
                if (_boomTimer <= diff)
                {
                    if (!_boomLocked)
                    {
                        DoCastAOE(SPELL_EXPLOSION);
                        me->DespawnOrUnsummon(0.2*IN_MILLISECONDS);
                        _boomLocked = true;
                    }
                }
                else
                    _boomTimer -= diff;
            }

            private:
                uint32 _boomTimer;
                bool _boomLocked;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_proximity_mineAI(creature);
        }
};

class spell_proximity_mines: public SpellScriptLoader // Spell 63027
{
    public:
        spell_proximity_mines() : SpellScriptLoader("spell_proximity_mines") {}

        class spell_proximity_mines_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_proximity_mines_SpellScript);

            void HandleDummyTick(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    for (uint8 i = 0; i < 10; i++)
                        caster->CastSpell(caster, SPELL_MINES_SPAWN, true);
                }
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_proximity_mines_SpellScript::HandleDummyTick, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_proximity_mines_SpellScript();
        }
};

class boss_vx_001 : public CreatureScript
{
    public:
        boss_vx_001() : CreatureScript("boss_vx_001") {}

        struct boss_vx_001AI : public Scripted_NoMovementAI
        {
            boss_vx_001AI(Creature* creature) : Scripted_NoMovementAI(creature) {}

            void InitializeAI()
            {
                _instance = me->GetInstanceScript();
                me->ApplySpellImmune(0, IMMUNITY_ID, SPELL_ROCKET_STRIKE_DMG, true);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }

            void Reset()
            {
                _spinning = false;
                _direction = false;
                _spinTimer = 0.25*IN_MILLISECONDS;

                _events.Reset();
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1 | UNIT_FLAG_DISABLE_MOVE);
                me->SetStandState(UNIT_STAND_STATE_STAND);
                me->RemoveAllAurasExceptType(SPELL_AURA_CONTROL_VEHICLE);
                _phase = PHASE_IDLE;
                _events.SetPhase(_phase);
                _mimironHardMode = false;
            }

            void KilledUnit(Unit* /*who*/)
            {
                if (_instance)
                    if (uint64 Mimiron = _instance->GetData64(BOSS_MIMIRON))
                    {
                        if (_phase == PHASE_VX001_SOLO__GLOBAL_2)
                        {
                            if (!urand(0,5))
                                Talk(SAY_VX001_SLAY, Mimiron);
                        }
                        else
                            if (!urand(0,5))
                                Talk(SAY_V07TRON_SLAY, Mimiron);
                    }
            }

            void EnterCombat(Unit* /*who*/)
            {
                if (Creature* Mimiron = ObjectAccessor::GetCreature(*me, _instance ? _instance->GetData64(BOSS_MIMIRON) : 0))
                    _mimironHardMode = Mimiron->AI()->GetData(DATA_GET_HARD_MODE);

                if (_mimironHardMode)
                {
                    DoCast(me, SPELL_EMERGENCY_MODE, true);
                    _events.ScheduleEvent(EVENT_FROST_BOMB, 15*IN_MILLISECONDS, 0, PHASE_VX001_SOLO__GLOBAL_2);
                    _events.ScheduleEvent(EVENT_FLAME_SUPPRESSANT_VX001, 1*IN_MILLISECONDS, 0, PHASE_VX001_SOLO__GLOBAL_2);
                }

                _events.ScheduleEvent(EVENT_RAPID_BURST, 2.5*IN_MILLISECONDS, 0, PHASE_VX001_SOLO__GLOBAL_2);
                _events.ScheduleEvent(EVENT_LASER_BARRAGE, urand(35*IN_MILLISECONDS, 40*IN_MILLISECONDS), 0, PHASE_VX001_SOLO__GLOBAL_2);
                _events.ScheduleEvent(EVENT_ROCKET_STRIKE, 20*IN_MILLISECONDS, 0, PHASE_VX001_SOLO__GLOBAL_2);
                _events.ScheduleEvent(EVENT_HEAT_WAVE, urand(8*IN_MILLISECONDS, 10*IN_MILLISECONDS), 0, PHASE_VX001_SOLO__GLOBAL_2);
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    case DO_START_VX001:
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1 | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE);
                        _phase = PHASE_VX001_SOLO__GLOBAL_2;
                        _events.SetPhase(_phase);
                        me->setFaction(14);
                        me->SetInCombatWithZone();
                        break;
                    case DO_VX001_ASSEMBLED:                                // Reassemble and heal share some stuff, fallthrough is intended
                        me->SetHealth( (me->GetMaxHealth() >> 1) );
                    case DO_VX001_SELF_REPAIR_END:
                        if (_mimironHardMode)
                            if (!me->HasAura(SPELL_EMERGENCY_MODE))
                                DoCast(me, SPELL_EMERGENCY_MODE, true);
                        _phase = PHASE_VX001_ASSEMBLED__GLOBAL_4;
                        _events.SetPhase(_phase);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1);
                        me->SetStandState(UNIT_STAND_STATE_STAND);
                        _events.RescheduleEvent(EVENT_LASER_BARRAGE, urand(35*IN_MILLISECONDS, 40*IN_MILLISECONDS), 0, PHASE_VX001_ASSEMBLED__GLOBAL_4);
                        _events.RescheduleEvent(EVENT_ROCKET_STRIKE, 20*IN_MILLISECONDS, 0, PHASE_VX001_ASSEMBLED__GLOBAL_4);
                        _events.RescheduleEvent(EVENT_HAND_PULSE, 5*IN_MILLISECONDS, 0, PHASE_VX001_ASSEMBLED__GLOBAL_4);
                        if (_mimironHardMode)
                            _events.RescheduleEvent(EVENT_FROST_BOMB, 15*IN_MILLISECONDS, 0, PHASE_VX001_ASSEMBLED__GLOBAL_4);
                        break;
                    case DO_ENTER_ENRAGE:
                        DoCast(me, SPELL_BERSERK, true);
                        break;
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* /*who*/, uint32 &damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (damage >= me->GetHealth())
                {
                    damage = 0;
                    _spinning = false;
                    me->InterruptNonMeleeSpells(true);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1);
                    me->AttackStop();
                    me->SetReactState(REACT_PASSIVE);
                    me->RemoveAllAurasExceptType(SPELL_AURA_CONTROL_VEHICLE);
                    me->SetStandState(UNIT_STAND_STATE_DEAD);
                    switch (_phase)
                    {
                        case PHASE_VX001_SOLO__GLOBAL_2:
                            me->SetHealth(me->GetMaxHealth());
                            if (Creature* Mimiron = ObjectAccessor::GetCreature(*me, _instance->GetData64(BOSS_MIMIRON)))
                                Mimiron->AI()->DoAction(DO_ACTIVATE_AERIAL);
                            break;
                        case PHASE_VX001_ASSEMBLED__GLOBAL_4:
                            if (Creature* Mimiron = ObjectAccessor::GetCreature(*me, _instance->GetData64(BOSS_MIMIRON)))
                                Mimiron->AI()->DoAction(DO_VX001_SELF_REPAIR_START);
                            DoCast(me, SPELL_SELF_REPAIR);
                            break;
                        default:
                            break;
                    }
                    _events.Reset();
                    _phase = PHASE_IDLE;
                    _events.SetPhase(_phase);
                }
            }

            void SpellHit(Unit* caster, SpellInfo const* spell)
            {
                if (!caster || !spell)
                    return;

                if (spell->Id == SPELL_SELF_REPAIR)
                {
                    DoAction(DO_VX001_SELF_REPAIR_END);
                    if (Creature* Mimiron = ObjectAccessor::GetCreature(*me, _instance->GetData64(BOSS_MIMIRON)))
                        Mimiron->AI()->DoAction(DO_VX001_SELF_REPAIR_END);
                }
            }

            void SpellHitTarget(Unit* target, SpellInfo const* spell)
            {
                switch (spell->Id)
                {
                    case SPELL_FROSTBOMB:
                        me->SummonCreature(NPC_FROST_BOMB, *target, TEMPSUMMON_TIMED_DESPAWN, 11*IN_MILLISECONDS);
                        break;
                    case SPELL_ROCKET_STRIKE:
                        if (!target || !spell)
                            return;

                        if (target->ToPlayer())
                            if (spell->Id == SPELL_ROCKET_STRIKE)
                                if (InstanceScript* instance = me->GetInstanceScript())
                                    if (Creature* mimiron = ObjectAccessor::GetCreature(*me, instance->GetData64(BOSS_MIMIRON)))
                                        mimiron->AI()->DoAction(DATA_AVOIDED_ROCKET_STRIKES);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim() || _phase == PHASE_IDLE)
                    return;

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (_spinning)
                {
                    if (_spinTimer <= diff)
                    {
                        float orient = me->GetOrientation() + (_direction ? M_PI/60.0f : -M_PI/60.0f);
                        if (Creature* leviathan = me->GetVehicleCreatureBase())
                        {
                            leviathan->SetFacingTo(orient);
                            me->SetOrientation(orient);
                        }
                        else
                        {
                            me->SetFacingTo(orient);
                            me->SetOrientation(orient);
                        }

                        float x, y;

                        me->GetNearPoint2D(x, y, 10.0f, orient);
                        if (Creature* temp = me->SummonCreature(NPC_BURST_TARGET, x, y, me->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN, 0.5*IN_MILLISECONDS))
                        {
                            me->SetTarget(temp->GetGUID());
                            if (me->IsVehicle())
                            {
                                // mimiron himself casts the spell, so lets turn him
                                if (Unit* mimiron = me->GetVehicleKit()->GetPassenger(1))
                                {
                                    mimiron->SetFacingTo(orient);
                                    mimiron->SetOrientation(orient);
                                    mimiron->SetTarget(temp->GetGUID());
                                }
                            }
                        }
                        _spinTimer = 0.25*IN_MILLISECONDS;
                    }
                    else
                        _spinTimer -= diff;
                }

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_RAPID_BURST:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                if (Creature* BurstTarget = me->SummonCreature(NPC_BURST_TARGET, *target, TEMPSUMMON_TIMED_DESPAWN, 3.1*IN_MILLISECONDS))
                                    DoCast(BurstTarget, SPELL_RAPID_BURST);
                            _events.RescheduleEvent(EVENT_RAPID_BURST, 5*IN_MILLISECONDS, 0, PHASE_VX001_SOLO__GLOBAL_2);
                            return;
                        case EVENT_LASER_BARRAGE:
                        {
                            float orient = float(2 * M_PI * rand_norm());
                            me->SetOrientation(orient);
                            me->SetFacingTo(orient);
                            me->SetReactState(REACT_PASSIVE);
                            if (Creature* leviathan = me->GetVehicleCreatureBase())
                            {
                                leviathan->CastSpell(leviathan, SPELL_SELF_STUN, true); // temporary
                                leviathan->SetFacingTo(orient);
                                if (Creature* AerialUnit = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_AERIAL_UNIT)))
                                    AerialUnit->SetFacingTo(orient);
                            }
                            // mimiron himself casts the spell, so turn him too
                            if (me->IsVehicle())
                            {
                                if (Unit* mimiron = me->GetVehicleKit()->GetPassenger(1))
                                {
                                    mimiron->SetOrientation(orient);
                                    mimiron->SetFacingTo(orient);
                                }
                            }
                            _direction = RAND(true, false);
                            _spinning = true;
                            DoCast(SPELL__spinning_UP); // The triggered spells should be casted on another target, seem to be self-casted 63274,66490
                            _events.DelayEvents(15*IN_MILLISECONDS);
                            _events.RescheduleEvent(EVENT_LASER_BARRAGE, 1*MINUTE*IN_MILLISECONDS, 0, _phase);
                            _events.RescheduleEvent(EVENT_LASER_BARRAGE_END, 14*IN_MILLISECONDS, 0, _phase);
                            return;
                        }
                        case EVENT_LASER_BARRAGE_END:
                            me->SetReactState(REACT_AGGRESSIVE);
                            if (me->getVictim())
                                AttackStart(me->getVictim());
                            else
                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                                    AttackStart(target);
                            _spinning = false;
                            return;
                        case EVENT_ROCKET_STRIKE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            {
                                if (Creature* pTemp = me->SummonCreature(NPC_MIMIRON_FOCUS, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 10*IN_MILLISECONDS))
                                {
                                    pTemp->SetDisplayId(pTemp->GetCreatureTemplate()->Modelid2);
                                    pTemp->GetMotionMaster()->MoveIdle();
                                    pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                                    pTemp->CombatStop();
                                    pTemp->CastSpell(pTemp,SPELL_ROCKET_STRIKE_AURA, true);
                                }
                                if (Unit* missile = me->GetVehicleKit()->GetPassenger(5))
                                    missile->CastSpell(target, SPELL_ROCKET_STRIKE, true);
                            }
                            if (_phase == PHASE_VX001_ASSEMBLED__GLOBAL_4)
                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                {
                                    if (Creature* pTemp = me->SummonCreature(NPC_MIMIRON_FOCUS, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 10*IN_MILLISECONDS))
                                    {
                                        pTemp->SetDisplayId(pTemp->GetCreatureTemplate()->Modelid2);
                                        pTemp->GetMotionMaster()->MoveIdle();
                                        pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                                        pTemp->CombatStop();
                                        pTemp->CastSpell(pTemp,SPELL_ROCKET_STRIKE_AURA,true);
                                    }
                                    if (Unit* missile = me->GetVehicleKit()->GetPassenger(6))
                                        missile->CastSpell(target, SPELL_ROCKET_STRIKE, true);
                                }
                            _events.RescheduleEvent(EVENT_ROCKET_STRIKE, urand(20*IN_MILLISECONDS, 25*IN_MILLISECONDS), 0, _phase);
                            return;
                        case EVENT_HEAT_WAVE:
                            DoCastAOE(SPELL_HEAT_WAVE);
                            _events.RescheduleEvent(EVENT_HEAT_WAVE, 10*IN_MILLISECONDS, 0, PHASE_VX001_SOLO__GLOBAL_2);
                            return;
                        case EVENT_HAND_PULSE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                DoCast(target, SPELL_HAND_PULSE);
                            _events.RescheduleEvent(EVENT_HAND_PULSE, urand(3*IN_MILLISECONDS, 4*IN_MILLISECONDS), 0 , PHASE_VX001_ASSEMBLED__GLOBAL_4);
                            return;
                        case EVENT_FROST_BOMB:
                            if (me->FindNearestCreature(NPC_FLAME_SPREAD, 100.0f))
                            {
                                DoCast(SPELL_FROSTBOMB);
                                _events.RescheduleEvent(EVENT_FROST_BOMB, 45*IN_MILLISECONDS, 0, _phase);
                            }
                            else
                                _events.RescheduleEvent(EVENT_FROST_BOMB, 5*IN_MILLISECONDS, 0, _phase);
                            return;
                        case EVENT_FLAME_SUPPRESSANT_VX001:
                            DoCastAOE(SPELL_FLAME_SUPPRESSANT_VX001);
                            _events.RescheduleEvent(EVENT_FLAME_SUPPRESSANT_VX001, 10*IN_MILLISECONDS, 0, PHASE_VX001_SOLO__GLOBAL_2);
                            return;
                        default:
                            return;
                    }
                }
            }

            private:
                Phases _phase;
                EventMap _events;
                InstanceScript* _instance;
                bool _mimironHardMode;
                bool _spinning;
                bool _direction;
                uint32 _spinTimer;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetUlduarAI<boss_vx_001AI>(creature);
        }
};

class npc_rocket_strike : public CreatureScript
{
    public:
        npc_rocket_strike() : CreatureScript("npc_rocket_strike") {}

        struct npc_rocket_strikeAI : public Scripted_NoMovementAI
        {
            npc_rocket_strikeAI(Creature* creature) : Scripted_NoMovementAI(creature) {}

            void InitializeAI()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1 | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
                Reset();
            }

            void Reset()
            {
                _casted = false;
            }

            void UpdateAI(uint32 const /*diff*/)
            {
                if (!_casted)
                {
                    DoCast(me, SPELL_ROCKET_STRIKE_AURA);
                    me->DespawnOrUnsummon(10*IN_MILLISECONDS);
                    _casted = true;
                }
            }

        private:
            bool _casted;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_rocket_strikeAI(creature);
        }
};

class spell_rapid_burst: public SpellScriptLoader
{
    public:
        spell_rapid_burst() : SpellScriptLoader("spell_rapid_burst") {}

        class spell_rapid_burst_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rapid_burst_AuraScript);

            void HandleDummyTick(AuraEffect const* aurEff)
            {
                if (!GetTarget())
                    return;

                if (Unit* caster = GetCaster())
                {
                    switch (caster->GetMap()->GetDifficultyID())
                    {
                        case Difficulty::RAID_DIFFICULTY_10MAN_NORMAL:
                            caster->CastSpell(GetTarget(), RAND(SPELL_RAPID_BURST_LEFT_10, SPELL_RAPID_BURST_RIGHT_10), true, NULL, aurEff);
                            break;
                        case Difficulty::RAID_DIFFICULTY_25MAN_NORMAL:
                            caster->CastSpell(GetTarget(), RAND(SPELL_RAPID_BURST_LEFT_25, SPELL_RAPID_BURST_RIGHT_25), true, NULL, aurEff);
                            break;
                        default:
                            break;
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_rapid_burst_AuraScript::HandleDummyTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rapid_burst_AuraScript();
        }
};

class boss_aerial_unit : public CreatureScript
{
    public:
        boss_aerial_unit() : CreatureScript("boss_aerial_unit") {}

        struct boss_aerial_unitAI : public ScriptedAI
        {
            boss_aerial_unitAI(Creature* creature) : ScriptedAI(creature), _summons(me), _phase(PHASE_IDLE) {}

            void InitializeAI()
            {
                _instance = me->GetInstanceScript();
                me->ApplySpellImmune(0, IMMUNITY_ID, SPELL_ROCKET_STRIKE_DMG, true);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->SetUnitMovementFlags(MOVEMENTFLAG_FLYING);
            }

            void Reset()
            {
                _events.Reset();
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1 | UNIT_FLAG_DISABLE_MOVE);
                me->SetReactState(REACT_PASSIVE);
                me->SetStandState(UNIT_STAND_STATE_STAND);
                me->SetVisible(false);
                me->RemoveAllAurasExceptType(SPELL_AURA_CONTROL_VEHICLE);
                me->SetCanFly(true);
                _phase = PHASE_IDLE;
                _events.SetPhase(PHASE_IDLE);
                _summons.DespawnAll();
                _gotMimironHardMode = false;
            }

            void KilledUnit(Unit* /*who*/)
            {
                if (_instance)
                    if (uint64 Mimiron = _instance->GetData64(BOSS_MIMIRON))
                    {
                        if (_phase == PHASE_AERIAL_SOLO__GLOBAL_3)
                        {
                            if (!urand(0,5))
                                Talk(SAY_AERIAL_SLAY, Mimiron);
                        }
                        else
                            if (!urand(0,5))
                                Talk(SAY_V07TRON_SLAY, Mimiron);
                    }
            }

            void SpellHit(Unit* caster, SpellInfo const* spell)
            {
                if (!caster || !spell)
                    return;

                if (spell->Id == SPELL_SELF_REPAIR)
                {
                    DoAction(DO_AERIAL_SELF_REPAIR_END);
                    if (Creature* Mimiron = ObjectAccessor::GetCreature(*me, _instance->GetData64(BOSS_MIMIRON)))
                        Mimiron->AI()->DoAction(DO_AERIAL_SELF_REPAIR_END);
                }
            }

            void EnterCombat(Unit* /*who*/)
            {
                if (Creature* Mimiron = ObjectAccessor::GetCreature(*me, _instance ? _instance->GetData64(BOSS_MIMIRON) : 0))
                    _gotMimironHardMode = Mimiron->AI()->GetData(DATA_GET_HARD_MODE);

                if (_gotMimironHardMode)
                    DoCast(me, SPELL_EMERGENCY_MODE, true);

                _events.ScheduleEvent(EVENT_PLASMA_BALL, 1*IN_MILLISECONDS);
                _events.ScheduleEvent(EVENT_SUMMON_JUNK_BOT, 10*IN_MILLISECONDS, 0, PHASE_AERIAL_SOLO__GLOBAL_3);
            }

            void AttackStart(Unit* target)
            {
                AttackStartCaster(target, 100.0f);
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    case DO_START_AERIAL:
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1 | UNIT_FLAG_IMMUNE_TO_PC);
                        me->SetReactState(REACT_AGGRESSIVE);
                        _phase = PHASE_AERIAL_SOLO__GLOBAL_3;
                        _events.SetPhase(PHASE_AERIAL_SOLO__GLOBAL_3);
                        DoZoneInCombat();
                        break;
                    case DO_DISABLE_AERIAL:
                        if (_phase == PHASE_AERIAL_SOLO__GLOBAL_3)
                        {
                            me->CastStop();
                            me->SetReactState(REACT_PASSIVE);
                            me->GetMotionMaster()->Clear();
                            // check if the aura works correctly (+50% damage from all sources)
                            DoCast(me, SPELL_MAGNETIC_CORE);
                            DoCast(me, SPELL_MAGNETIC_CORE_VISUAL);
                            // Move to floor.
                            Position destination;
                            me->GetPosition(&destination);
                            destination.m_positionZ = 368.965f;
                            me->GetMotionMaster()->MoveLand(1, destination);  // Check if MoveLand is ok here, a flying unit should have a landing animation, but... just 4 the case
                            _events.DelayEvents(20*IN_MILLISECONDS);
                            _events.ScheduleEvent(EVENT_REACTIVATE_AERIAL, 20*IN_MILLISECONDS, 0, PHASE_AERIAL_SOLO__GLOBAL_3);
                        }
                        break;
                    case DO_AERIAL_ASSEMBLED:
                        me->SetHealth( (me->GetMaxHealth() >> 1) );               // Once again, re-assemble and repairing share some stuff, so the fallthrough is intended!
                    case DO_AERIAL_SELF_REPAIR_END:
                        if (_gotMimironHardMode)
                            if (!me->HasAura(SPELL_EMERGENCY_MODE))
                                DoCast(me, SPELL_EMERGENCY_MODE, true);
                        _phase = PHASE_AERIAL_ASSEMBLED__GLOBAL_4;
                        _events.SetPhase(_phase);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1);
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->SetStandState(UNIT_STAND_STATE_STAND);
                        _events.RescheduleEvent(EVENT_PLASMA_BALL, 2*IN_MILLISECONDS);
                        break;
                    case DO_ENTER_ENRAGE:
                        DoCast(me, SPELL_BERSERK, true);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim() || _phase == PHASE_IDLE)
                    return;

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_PLASMA_BALL:
                            if (_phase == PHASE_AERIAL_SOLO__GLOBAL_3 && me->getVictim())
                            {
                                float x = me->getVictim()->GetPositionX();
                                float y = me->getVictim()->GetPositionY();
                                float z = me->getVictim()->GetPositionZ();
                                if (me->IsWithinDist3d(x, y, z, 30.0f))
                                {
                                    me->GetMotionMaster()->Initialize();
                                    DoCastVictim(SPELL_PLASMA_BALL);
                                }
                                else
                                    me->GetMotionMaster()->MovePoint(0, x, y, 380.04f);
                            }
                            else if (_phase == PHASE_AERIAL_ASSEMBLED__GLOBAL_4 && me->getVictim())
                            {
                                if (me->getVictim()->IsWithinDist3d(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 30.0f))
                                    DoCastVictim(SPELL_PLASMA_BALL);
                                else if (Unit* target = SelectTarget(SELECT_TARGET_NEAREST, 0, 0.0f, true))
                                    DoCast(target, SPELL_PLASMA_BALL);
                            }
                            _events.RescheduleEvent(EVENT_PLASMA_BALL, 2*IN_MILLISECONDS);
                            return;
                        case EVENT_REACTIVATE_AERIAL:
                            me->RemoveAurasDueToSpell(SPELL_MAGNETIC_CORE_VISUAL);
                            Position destination;
                            me->GetPosition(&destination);
                            destination.m_positionZ = 380.04f;
                            me->GetMotionMaster()->MoveTakeoff(1, destination); // Check if MoveTakeoff is ok here, a flying unit should have a landing animation, but... just 4 the case
                            me->SetReactState(REACT_AGGRESSIVE);
                            return;
                        case EVENT_SUMMON_JUNK_BOT:
                            for (uint8 n = 0; n < 2; n++)
                                me->SummonCreature(NPC_JUNK_BOT, SummonPos[rand()%9], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3*IN_MILLISECONDS);
                            _events.RescheduleEvent(EVENT_SUMMON_ASSAULT_BOT, 10*IN_MILLISECONDS, 0, PHASE_AERIAL_SOLO__GLOBAL_3);
                            return;
                        case EVENT_SUMMON_ASSAULT_BOT:
                            me->SummonCreature(NPC_ASSAULT_BOT, SummonPos[rand()%9], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30*IN_MILLISECONDS);
                            if (_gotMimironHardMode)
                                for (uint8 i = 0; i < 2; i++)
                                    me->SummonCreature(NPC_EMERGENCY_BOT, SummonPos[rand()%9], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3*IN_MILLISECONDS);
                            _events.RescheduleEvent(EVENT_SUMMON_BOOM_BOT, 10*IN_MILLISECONDS, 0, PHASE_AERIAL_SOLO__GLOBAL_3);
                            return;
                        case EVENT_SUMMON_BOOM_BOT:
                            me->SummonCreature(NPC_BOOM_BOT, 2744.65f, 2569.46f, 364.397f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3*IN_MILLISECONDS);
                            _events.RescheduleEvent(EVENT_SUMMON_JUNK_BOT, 10*IN_MILLISECONDS, 0, PHASE_AERIAL_SOLO__GLOBAL_3);
                            return;
                        default:
                            return;
                    }
                }
            }

            void JustSummoned(Creature* summon)
            {
                _summons.Summon(summon);

                if (summon->GetEntry() == NPC_EMERGENCY_BOT)
                    return;

                summon->AI()->DoZoneInCombat();
                if (_gotMimironHardMode)
                    summon->CastSpell(summon, SPELL_EMERGENCY_MODE, true);
            }

            void DamageTaken(Unit* /*who*/, uint32 &damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (damage >= me->GetHealth())
                {
                    damage = 0;
                    me->InterruptNonMeleeSpells(true);
                    me->GetMotionMaster()->Clear(true);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1);
                    me->SetReactState(REACT_PASSIVE);
                    me->AttackStop();
                    me->RemoveAllAurasExceptType(SPELL_AURA_CONTROL_VEHICLE);
                    me->SetStandState(UNIT_STAND_STATE_DEAD);
                    switch (_phase)
                    {
                        case PHASE_AERIAL_SOLO__GLOBAL_3:
                            me->SetHealth(me->GetMaxHealth());
                            _events.CancelEvent(EVENT_SUMMON_JUNK_BOT);
                            if (Creature* Mimiron = ObjectAccessor::GetCreature(*me, _instance->GetData64(BOSS_MIMIRON)))
                                Mimiron->AI()->DoAction(DO_ACTIVATE_V0L7R0N);
                            break;
                        case PHASE_AERIAL_ASSEMBLED__GLOBAL_4:
                            if (Creature* Mimiron = ObjectAccessor::GetCreature(*me, _instance->GetData64(BOSS_MIMIRON)))
                                Mimiron->AI()->DoAction(DO_AERIAL_SELF_REPAIR_START);
                            DoCast(me, SPELL_SELF_REPAIR);
                            break;
                        default:
                            break;
                    }
                    _events.Reset();
                    _phase = PHASE_IDLE;
                    _events.SetPhase(PHASE_IDLE);
                }
            }

            private:
                Phases _phase;
                EventMap _events;
                SummonList _summons;
                InstanceScript* _instance;
                bool _gotMimironHardMode;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetUlduarAI<boss_aerial_unitAI>(creature);
        }
};

class npc_magnetic_core : public CreatureScript
{
    public:
        npc_magnetic_core() : CreatureScript("npc_magnetic_core") {}

        struct npc_magnetic_coreAI : public Scripted_NoMovementAI
        {
            npc_magnetic_coreAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
                me->DespawnOrUnsummon(21*IN_MILLISECONDS);
                if (Creature* AerialUnit = me->FindNearestCreature(NPC_AERIAL_COMMAND_UNIT, 100.0f, true))
                {
                    AerialUnit->AI()->DoAction(DO_DISABLE_AERIAL); // Causes the NPC to land
                    // me->GetMotionMaster()->MoveFall();
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_magnetic_coreAI(creature);
        }
};

class npc_assault_bot : public CreatureScript
{
    public:
        npc_assault_bot() : CreatureScript("npc_assault_bot") {}

        struct npc_assault_botAI : public ScriptedAI
        {
            npc_assault_botAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                _fieldTimer = urand(4*IN_MILLISECONDS, 6*IN_MILLISECONDS);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                if (_fieldTimer <= diff)
                {
                    DoCastVictim(SPELL_MAGNETIC_FIELD);
                    _fieldTimer = urand(15*IN_MILLISECONDS, 20*IN_MILLISECONDS);
                }
                else
                    _fieldTimer -= diff;

                DoMeleeAttackIfReady();
            }

            void SpellHit(Unit* /*caster*/, SpellInfo const* spell)
            {
                // Achievement Not-So-Friendly Fire
                if (spell->Id == SPELL_ROCKET_STRIKE_DMG)
                    if (Player* player = me->SelectNearestPlayer(100.0f))
                        player->CastSpell(player, SPELL_NOT_SO_FRIENDLY_FIRE, true);
            }

            private:
                uint32 _fieldTimer;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_assault_botAI(creature);
        }
};

class npc_emergency_bot : public CreatureScript
{
    public:
        npc_emergency_bot() : CreatureScript("npc_emergency_bot") {}

        struct npc_emergency_botAI : public ScriptedAI
        {
            npc_emergency_botAI(Creature* creature) : ScriptedAI(creature) {}

            void InitializeAI()
            {
                me->setFaction(14);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_ID, SPELL_DEATH_GRIP, true);
                me->SetReactState(REACT_PASSIVE);
                Reset();
            }

            void Reset()
            {
                me->GetMotionMaster()->MoveRandom(15);
                _sprayTimer = 5*IN_MILLISECONDS;

                if (Is25ManRaid())
                    DoCast(me, SPELL_DEAFENING_SIREN, true);
            }

            void UpdateAI(uint32 const diff)
            {
                if (_sprayTimer <= diff)
                {
                    DoCast(SPELL_WATER_SPRAY);
                    _sprayTimer = 10*IN_MILLISECONDS;
                }
                else
                    _sprayTimer -= diff;
            }

            private:
                uint32 _sprayTimer;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_emergency_botAI(creature);
        }
};

class npc_mimiron_bomb_bot : public CreatureScript
{
    public:
        npc_mimiron_bomb_bot() : CreatureScript("npc_mimiron_bomb_bot") {}

        struct npc_mimiron_bomb_botAI : public ScriptedAI
        {
            npc_mimiron_bomb_botAI(Creature* creature) : ScriptedAI(creature) {}

            void InitializeAI()
            {
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
                Reset();
            }

            Unit* SelectPlayerTargetInRange(float range)
            {
                Player* target = 0;
                Trinity::AnyPlayerInObjectRangeCheck u_check(me, range, true);
                Trinity::PlayerSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(me, target, u_check);
                me->VisitNearbyObject(range, searcher);
                return target;
            }

            void Reset()
            {
                _despawn = false;

                if (Unit* target = SelectPlayerTargetInRange(100.0f))
                {
                    me->AddThreat(target, std::numeric_limits<float>::max());
                    me->GetMotionMaster()->MoveFollow(target, 100.0f, 0.0f);
                }
            }

            void SpellHitTarget(Unit* target, SpellInfo const* spell)
            {
                if (!target || !spell)
                    return;

                if (target->ToPlayer())
                    if (spell->Id == SPELL_BOOM_BOT || spell->Id == SPELL_BOOM_BOT_PERIODIC)
                        if (InstanceScript* instance = me->GetInstanceScript())
                            if (Creature* mimiron = ObjectAccessor::GetCreature(*me, instance->GetData64(BOSS_MIMIRON)))
                                mimiron->AI()->DoAction(DATA_AVOIDED_BOOM_BOT_EXPLOSION);
            }

            void SpellHit(Unit* /*caster*/, SpellInfo const* spell)
            {
                if (spell->Id == SPELL_BOOM_BOT_PERIODIC)
                    me->DespawnOrUnsummon(1*IN_MILLISECONDS);
            }

            void JustDied(Unit* /*killer*/)
            {
                DoCast(me, SPELL_BOOM_BOT, true);
            }

            void UpdateAI(const uint32 /*diff*/)
            {
                if (!UpdateVictim())
                    return;

                if (!_despawn && me->IsWithinMeleeRange(me->getVictim()))
                {
                    _despawn = true;
                    me->CastSpell(me, SPELL_BOOM_BOT, true);
                }
                // suicide has procflag PROC_FLAG_DONE_MELEE_AUTO_ATTACK, they have to melee, even tho the spell is delayed if the npc misses
                DoMeleeAttackIfReady();
            }

            private:
                bool _despawn;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_mimiron_bomb_botAI(creature);
        }
};

class go_not_push_button : public GameObjectScript
{
    public:
        go_not_push_button() : GameObjectScript("go_mimiron_hard_mode_activator") {}

        bool OnGossipHello(Player* player, GameObject* go)
        {
            if (InstanceScript* instance = go->GetInstanceScript())
            {
                if ((instance->GetBossState(BOSS_MIMIRON) != IN_PROGRESS && instance->GetBossState(BOSS_MIMIRON) != DONE) && player)
                    if (Creature* mimiron = ObjectAccessor::GetCreature((*player), instance->GetData64(BOSS_MIMIRON)))
                    {
                        mimiron->AI()->DoAction(DO_ACTIVATE_HARD_MODE);
                        go->UseDoorOrButton();
                        return true;
                    }
            }
            return false;
        }
};

class npc_mimiron_flame_trigger : public CreatureScript
{
    public:
        npc_mimiron_flame_trigger() : CreatureScript("npc_mimiron_flame_trigger") {}

        struct npc_mimiron_flame_triggerAI : public ScriptedAI
        {
            npc_mimiron_flame_triggerAI(Creature* creature) : ScriptedAI(creature) {}

            void InitializeAI()
            {
                _instance = me->GetInstanceScript();
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1 | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
                Reset();
            }

            void Reset()
            {
                _flameTimer = 2*IN_MILLISECONDS;
                me->SetInCombatWithZone();
            }

            void SpellHit(Unit* /*caster*/, SpellInfo const* spell)
            {
                switch (spell->Id)
                {
                    case SPELL_FLAME_SUPPRESSANT_MK:
                    case SPELL_FLAME_SUPPRESSANT_VX001:
                    case SPELL_FROST_BOMB_EXPLOSION_10:
                    case SPELL_FROST_BOMB_EXPLOSION_25:
                    case SPELL_WATER_SPRAY:
                        _flameTimer = 1*IN_MILLISECONDS;
                        me->DespawnOrUnsummon(0.5*IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }
            }

            void JustSummoned(Creature* /*summon*/)
            {
                if (Creature* mimiron = ObjectAccessor::GetCreature(*me, _instance ? _instance->GetData64(BOSS_MIMIRON) : 0))
                    mimiron->AI()->DoAction(DO_INCREASE_FLAME_COUNT);
            }

            void UpdateAI(uint32 const diff)
            {
                if (_flameTimer <= diff)
                {
                    // check if flame cap is reached
                    if (Creature* mimiron = ObjectAccessor::GetCreature(*me, _instance ? _instance->GetData64(BOSS_MIMIRON) : 0))
                        if (mimiron->AI()->GetData(DATA_FLAME_COUNT) >= FLAME_CAP)
                        {
                            me->DespawnOrUnsummon();
                            return;
                        }

                    DoZoneInCombat(me, 100.0f);

                    if (Player* nearest = me->SelectNearestPlayer(100.0f))
                    {
                        me->GetMotionMaster()->Clear();
                        me->GetMotionMaster()->MoveFollow(nearest, 0.0f, 0.0f);
                    }

                    me->SummonCreature(NPC_FLAME_SPREAD, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                    _flameTimer = 4*IN_MILLISECONDS;
                }
                else
                    _flameTimer -= diff;
            }

            private:
                InstanceScript* _instance;
                uint32 _flameTimer;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_mimiron_flame_triggerAI(creature);
        }
};

class npc_mimiron_flame_spread : public CreatureScript
{
    public:
        npc_mimiron_flame_spread() : CreatureScript("npc_mimiron_flame_spread") {}

        struct npc_mimiron_flame_spreadAI : public ScriptedAI
        {
            npc_mimiron_flame_spreadAI(Creature* creature) : ScriptedAI(creature) {}

            void InitializeAI()
            {
                _instance = me->GetInstanceScript();
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1 | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED | UNIT_FLAG_DISABLE_MOVE);
                me->SetReactState(REACT_PASSIVE);
                Reset();
            }

            void Reset()
            {
                DoCast(me, SPELL_FLAME, true);
            }

            void SpellHit(Unit* /*caster*/, SpellInfo const* spell)
            {
                switch (spell->Id)
                {
                    case SPELL_FLAME_SUPPRESSANT_MK:
                    case SPELL_FLAME_SUPPRESSANT_VX001:
                    case SPELL_FROST_BOMB_EXPLOSION_10:
                    case SPELL_FROST_BOMB_EXPLOSION_25:
                    case SPELL_WATER_SPRAY:
                        if (Creature* mimiron = ObjectAccessor::GetCreature(*me, _instance ? _instance->GetData64(BOSS_MIMIRON) : 0))
                            mimiron->AI()->DoAction(DO_DECREASE_FLAME_COUNT);
                        me->DespawnOrUnsummon(0.5*IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const /*diff*/)
            {
                if (_instance && _instance->GetBossState(BOSS_MIMIRON) != IN_PROGRESS)
                    me->DespawnOrUnsummon();
            }

            private:
                InstanceScript* _instance;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_mimiron_flame_spreadAI(creature);
        }
};

class npc_frost_bomb : public CreatureScript
{
    public:
        npc_frost_bomb() : CreatureScript("npc_frost_bomb") {}

        struct npc_frost_bombAI : public Scripted_NoMovementAI
        {
            npc_frost_bombAI(Creature* creature) : Scripted_NoMovementAI(creature) {}

            void InitializeAI()
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1 | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
                Reset();
            }

            void Reset()
            {
                DoCast(me, SPELL_FROST_BOMB_VISUAL, true);
                _frostTimer = 10*IN_MILLISECONDS;
            }

            void UpdateAI(uint32 const diff)
            {
                if (_frostTimer <= diff)
                {
                    DoCast(me, RAID_MODE(SPELL_FROST_BOMB_EXPLOSION_10, SPELL_FROST_BOMB_EXPLOSION_25), true);
                    _frostTimer = 10*IN_MILLISECONDS;
                }
                else
                    _frostTimer -= diff;
            }

            private:
                uint32 _frostTimer;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_frost_bombAI(creature);
        }
};

class spell_frost_bomb: public SpellScriptLoader
{
    public:
        spell_frost_bomb() : SpellScriptLoader("spell_frost_bomb") {}

        class spell_frost_bomb_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_frost_bomb_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                _target = Trinity::Containers::SelectRandomContainerElement(targets);
                targets.clear();
                targets.push_back(_target);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_frost_bomb_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            }

            WorldObject* _target;
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_frost_bomb_SpellScript();
        }
};

class achievement_firefighter : public AchievementCriteriaScript
{
    public:
        achievement_firefighter(const char* name) : AchievementCriteriaScript(name) {}

        bool OnCheck(Player* player, Unit* /*target*/)
        {
            if (player)
                if (InstanceScript* instance = player->GetInstanceScript())
                    if (Creature* mimiron = ObjectAccessor::GetCreature(*player, instance->GetData64(BOSS_MIMIRON)))
                        return (mimiron->AI()->GetData(DATA_GET_HARD_MODE));
            return false;
        }
};

class achievement_set_up_us_the_bomb__proximity_mines : public AchievementCriteriaScript
{
    public:
        achievement_set_up_us_the_bomb__proximity_mines(const char* name) : AchievementCriteriaScript(name) {}

        bool OnCheck(Player* player, Unit* /*target*/)
        {
            if (player)
                if (InstanceScript* instance = player->GetInstanceScript())
                    if (Creature* mimiron = ObjectAccessor::GetCreature(*player, instance->GetData64(BOSS_MIMIRON)))
                        return (mimiron->AI()->GetData(DATA_AVOIDED_PROXIMITY_MINES));
            return false;
        }
};

class achievement_set_up_us_the_bomb__rocket_strikes : public AchievementCriteriaScript
{
    public:
        achievement_set_up_us_the_bomb__rocket_strikes(const char* name) : AchievementCriteriaScript(name) {}

        bool OnCheck(Player* player, Unit* /*target*/)
        {
            if (player)
                if (InstanceScript* instance = player->GetInstanceScript())
                    if (Creature* mimiron = ObjectAccessor::GetCreature(*player, instance->GetData64(BOSS_MIMIRON)))
                        return (mimiron->AI()->GetData(DATA_AVOIDED_ROCKET_STRIKES));
            return false;
        }
};

class achievement_set_up_us_the_bomb__boombot_explosion : public AchievementCriteriaScript
{
    public:
        achievement_set_up_us_the_bomb__boombot_explosion(const char* name) : AchievementCriteriaScript(name) {}

        bool OnCheck(Player* player, Unit* /*target*/)
        {
            if (player)
                if (InstanceScript* instance = player->GetInstanceScript())
                    if (Creature* mimiron = ObjectAccessor::GetCreature(*player, instance->GetData64(BOSS_MIMIRON)))
                        return (mimiron->AI()->GetData(DATA_AVOIDED_BOOM_BOT_EXPLOSION));
            return false;
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_mimiron()
{
    new boss_mimiron();
    new boss_leviathan_mk();
    new boss_leviathan_mk_turret();
    new boss_vx_001();
    new boss_aerial_unit();

    new npc_proximity_mine();
    new npc_rocket_strike();
    new npc_magnetic_core();
    new npc_assault_bot();
    new npc_emergency_bot();
    new npc_mimiron_bomb_bot();
    new npc_mimiron_flame_trigger();
    new npc_mimiron_flame_spread();
    new npc_frost_bomb();

    new spell_rapid_burst();
    new spell_proximity_mines();
    new spell_frost_bomb();

    new go_not_push_button();

    new achievement_firefighter("achievement_firefighter");     // Achiev 3180 / Criteria 10450
    new achievement_firefighter("achievement_firefighter_25");  // Achiev 3189 / Criteria 10463

    // TODO: Find correct criterias for the following achievements:
    new achievement_set_up_us_the_bomb__boombot_explosion("achievement_set_up_us_the_bomb__boombot");
    new achievement_set_up_us_the_bomb__boombot_explosion("achievement_set_up_us_the_bomb__boombot_25");
    new achievement_set_up_us_the_bomb__proximity_mines("achievement_set_up_us_the_bomb__proximity");
    new achievement_set_up_us_the_bomb__proximity_mines("achievement_set_up_us_the_bomb__proximity_25");
    new achievement_set_up_us_the_bomb__rocket_strikes("achievement_set_up_us_the_bomb__rockets");
    new achievement_set_up_us_the_bomb__rocket_strikes("achievement_set_up_us_the_bomb__rockets_25");
}
#endif
