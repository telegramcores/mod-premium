#include "Config.h"
#include "Define.h"
#include "GossipDef.h"
#include "Item.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "Spell.h"

enum Vendors
{
    NPC_VENDOR_A    = 54,
    NPC_VENDOR_H    = 3163,
    NPC_AUCTION_H   = 9856,
    NPC_AUCTION_A   = 8670
};

enum Trainers
{
    // Alliance
    DRUID_A     = 5504,
    HUNTER_A    = 5515,
    MAGE_A      = 5497,
    PALADIN_A   = 928,
    PRIEST_A    = 376,
    ROGUE_A     = 918,
    SHAMAN_A    = 20407,
    WARLOCK_A   = 461,
    WARRIOR_A   = 5479,

    // Horde
    DRUID_H     = 3033,
    HUNTER_H    = 3406,
    MAGE_H      = 5883,
    PALADIN_H   = 23128,
    PRIEST_H    = 3045,
    ROGUE_H     = 3401,
    SHAMAN_H    = 3344,
    WARLOCK_H   = 3324,
    WARRIOR_H   = 3354,

    DEATHKNIGHT_AH  = 28472
};

enum Mounts
{
    HUMAN_MOUNT     = 470,
    ORC_MOUNT       = 6653,
    GNOME_MOUNT     = 17454,
    NIGHTELF_MOUNT  = 8394,
    DWARF_MOUNT     = 6899,
    UNEAD_MOUNT     = 17463,
    TAUREN_MOUNT    = 64657,
    TROLL_MOUNT     = 8395,
    BLOODELF_MOUNT  = 35022,
    DRAENEI_MOUNT   = 34406
};

enum PremiumGossip
{
    PREMIUM_MENU = 62001,
    PREMIUM_MENU_TEXT = 90003,
    GOSSIP_MORPH = 0,
    GOSSIP_DEMORPH,
    GOSSIP_MOUNT,
    GOSSIP_TRAIN_ME,
    GOSSIP_PLAYER,
    GOSSIP_VENDOR,
    GOSSIP_MAIL,
    GOSSIP_BANK,
    GOSSIP_AUCTION_HOUSE,
    GOSSIP_FACTION
};

class premium_account : public ItemScript
{
public:
    premium_account() : ItemScript("premium_account") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
    {
        if (!sConfigMgr->GetOption<bool>("PremiumAccount", true))
            return false;

     /*   QueryResult result = CharacterDatabase.Query("SELECT `AccountId` FROM `premium` WHERE `active`=1 AND `AccountId`={}", player->GetSession()->GetAccountId()); 

        if (!result)
            return false;
     */
        if (player->IsInCombat())
            return false;

        float rangeCheck = 10.0f;

        if (player->FindNearestCreature(NPC_AUCTION_A, rangeCheck) ||
            player->FindNearestCreature(NPC_AUCTION_H, rangeCheck) ||
            player->FindNearestCreature(NPC_VENDOR_A, rangeCheck) ||
            player->FindNearestCreature(NPC_VENDOR_H, rangeCheck) ||
            player->FindNearestCreature(ROGUE_A, rangeCheck) ||
            player->FindNearestCreature(WARRIOR_A, rangeCheck) ||
            player->FindNearestCreature(HUNTER_A, rangeCheck) ||
            player->FindNearestCreature(PRIEST_A, rangeCheck) ||
            player->FindNearestCreature(PALADIN_A, rangeCheck) ||
            player->FindNearestCreature(DRUID_A, rangeCheck) ||
            player->FindNearestCreature(SHAMAN_A, rangeCheck) ||
            player->FindNearestCreature(MAGE_A, rangeCheck) ||
            player->FindNearestCreature(WARLOCK_A, rangeCheck) ||
            player->FindNearestCreature(HUNTER_H, rangeCheck) ||
            player->FindNearestCreature(WARRIOR_H, rangeCheck) ||
            player->FindNearestCreature(SHAMAN_H, rangeCheck) ||
            player->FindNearestCreature(PALADIN_H, rangeCheck) ||
            player->FindNearestCreature(ROGUE_H, rangeCheck) ||
            player->FindNearestCreature(WARLOCK_H, rangeCheck) ||
            player->FindNearestCreature(MAGE_H, rangeCheck) ||
            player->FindNearestCreature(PRIEST_H, rangeCheck) ||
            player->FindNearestCreature(DRUID_H, rangeCheck) ||
            player->FindNearestCreature(DEATHKNIGHT_AH, rangeCheck))
            return false;

        ClearGossipMenuFor(player);

        if (sConfigMgr->GetOption<bool>("Morph", true))
        {
            AddGossipItemFor(player, PREMIUM_MENU, GOSSIP_MORPH, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            AddGossipItemFor(player, PREMIUM_MENU, GOSSIP_DEMORPH, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
        }

        if (sConfigMgr->GetOption<bool>("Mount", true))
            AddGossipItemFor(player, PREMIUM_MENU, GOSSIP_MOUNT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);

        if (sConfigMgr->GetOption<bool>("Trainers", true))
            AddGossipItemFor(player, PREMIUM_MENU, GOSSIP_TRAIN_ME, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);

        if (sConfigMgr->GetOption<bool>("PlayerInteraction", true))
            AddGossipItemFor(player, PREMIUM_MENU, GOSSIP_PLAYER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);

        SendGossipMenuFor(player, PREMIUM_MENU_TEXT, item->GetGUID());
        return false; // Cast the spell on use normally
    }

    void OnGossipSelect(Player* player, Item* item, uint32 /*sender*/, uint32 action) override
    {
        switch (action)
        {
            case GOSSIP_ACTION_INFO_DEF + 1: /*Morph*/
            {
                CloseGossipMenuFor(player);
                ApplyRandomMorph(player);
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 2: /*Demorph*/
            {
                player->DeMorph();
                CloseGossipMenuFor(player);
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 3: /*Show Bank*/
            {
                player->GetSession()->SendShowBank(player->GetGUID());
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 4: /*Mail Box*/
            {
                player->GetSession()->SendShowMailBox(player->GetGUID());
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 5: /*Vendor*/
            {
                uint32 vendorId = 0;
                std::string salute;

                if (player->GetTeamId() == TEAM_ALLIANCE)
                {
                    vendorId = NPC_VENDOR_A;
                    switch (player->GetSession()->GetSessionDbLocaleIndex())
                    {
                    case LOCALE_enUS:
                    case LOCALE_koKR:
                    case LOCALE_frFR:
                    case LOCALE_deDE:
                    case LOCALE_zhCN:
                    case LOCALE_zhTW:
                    case LOCALE_ruRU:
                    {
                        salute = "Greetings";
                        break;
                    }
                    case LOCALE_esES:
                    case LOCALE_esMX:
                    {
                        salute = "Saludos.";
                    }
                    default:
                        break;
                    }
                }
                else
                {
                    vendorId = NPC_VENDOR_H;
                    salute = "Zug zug";
                }

                SummonTempNPC(player, vendorId, salute.c_str());
                CloseGossipMenuFor(player);
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 6: /*Mount*/
            {
                CloseGossipMenuFor(player);
                switch (player->getRace())
                {
                    case RACE_HUMAN:         player->CastSpell(player, HUMAN_MOUNT); break;
                    case RACE_ORC:           player->CastSpell(player, ORC_MOUNT); break;
                    case RACE_GNOME:         player->CastSpell(player, GNOME_MOUNT); break;
                    case RACE_NIGHTELF:      player->CastSpell(player, NIGHTELF_MOUNT); break;
                    case RACE_DWARF:         player->CastSpell(player, DWARF_MOUNT); break;
                    case RACE_DRAENEI:       player->CastSpell(player, DRAENEI_MOUNT); break;
                    case RACE_UNDEAD_PLAYER: player->CastSpell(player, UNEAD_MOUNT); break;
                    case RACE_TAUREN:        player->CastSpell(player, TAUREN_MOUNT); break;
                    case RACE_TROLL:         player->CastSpell(player, TROLL_MOUNT); break;
                    case RACE_BLOODELF:      player->CastSpell(player, BLOODELF_MOUNT); break;
                }
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 7: /*Auction House*/
            {
                uint32 auctionId = 0;
                std::string salute = "";
                if (player->GetTeamId() == TEAM_HORDE)
                {
                    auctionId = NPC_AUCTION_H;
                    switch (player->GetSession()->GetSessionDbLocaleIndex())
                    {
                    case LOCALE_enUS:
                    case LOCALE_koKR:
                    case LOCALE_frFR:
                    case LOCALE_deDE:
                    case LOCALE_zhCN:
                    case LOCALE_zhTW:
                    case LOCALE_ruRU:
                    {
                        salute = "Привет, дававай быстрее, мне нужно возвращаться в Оргримар!";
                        break;
                    }
                    case LOCALE_esES:
                    case LOCALE_esMX:
                    {
                        salute = "Me iré en breve, necesito volver a Orgrimmar.";
                    }
                    default:
                        break;
                    }
                }
                else
                {
                    auctionId = NPC_AUCTION_A;
                    switch (player->GetSession()->GetSessionDbLocaleIndex())
                    {
                    case LOCALE_enUS:
                    case LOCALE_koKR:
                    case LOCALE_frFR:
                    case LOCALE_deDE:
                    case LOCALE_zhCN:
                    case LOCALE_zhTW:
                    case LOCALE_ruRU:
                    {
                        salute = "Привет, дававай быстрее, мне нужно возвращаться в Штормград!";
                        break;
                    }
                    case LOCALE_esES:
                    case LOCALE_esMX:
                    {
                        salute = "Me iré en breve, necesito volver a la Ciudad de Ventormenta.";
                    }
                    default:
                        break;
                    }
                }

                SummonTempNPC(player, auctionId, salute.c_str());
                CloseGossipMenuFor(player);
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 8: /* Class Trainers*/
            {
                uint32 trainerId = 0;
                switch (player->getClass())
                {
                    case CLASS_ROGUE:
                        trainerId = player->GetTeamId() == TEAM_ALLIANCE ? ROGUE_A : ROGUE_H;
                        break;
                    case CLASS_WARRIOR:
                        trainerId = player->GetTeamId() == TEAM_ALLIANCE ? WARRIOR_A : WARRIOR_H;
                        break;
                    case CLASS_PRIEST:
                        trainerId = player->GetTeamId() == TEAM_ALLIANCE ? PRIEST_A : PRIEST_H;
                        break;
                    case CLASS_MAGE:
                        trainerId = player->GetTeamId() == TEAM_ALLIANCE ? MAGE_A : MAGE_H;
                        break;
                    case CLASS_PALADIN:
                        trainerId = player->GetTeamId() == TEAM_ALLIANCE ? PALADIN_A : PALADIN_H;
                        break;
                    case CLASS_HUNTER:
                        trainerId = player->GetTeamId() == TEAM_ALLIANCE ? HUNTER_A : HUNTER_H;
                        break;
                    case CLASS_DRUID:
                        trainerId = player->GetTeamId() == TEAM_ALLIANCE ? DRUID_A : DRUID_H;
                        break;
                    case CLASS_SHAMAN:
                        trainerId = player->GetTeamId() == TEAM_ALLIANCE ? SHAMAN_A : SHAMAN_H;
                        break;
                    case CLASS_WARLOCK:
                        trainerId = player->GetTeamId() == TEAM_ALLIANCE ? WARLOCK_A : WARLOCK_H;
                        break;
                    case CLASS_DEATH_KNIGHT:
                        trainerId = DEATHKNIGHT_AH;
                        break;
                }

                SummonTempNPC(player, trainerId);
                CloseGossipMenuFor(player);
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 9: /*Player Interactions*/
            {
                ClearGossipMenuFor(player);

                if (sConfigMgr->GetOption<bool>("Vendor", true))
                    AddGossipItemFor(player, PREMIUM_MENU, GOSSIP_VENDOR, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);

                if (sConfigMgr->GetOption<bool>("MailBox", true))
                    AddGossipItemFor(player, PREMIUM_MENU, GOSSIP_MAIL, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);

                if (sConfigMgr->GetOption<bool>("Bank", true))
                    AddGossipItemFor(player, PREMIUM_MENU, GOSSIP_BANK, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);

                if (sConfigMgr->GetOption<bool>("Auction", true))
                    AddGossipItemFor(player, PREMIUM_MENU, GOSSIP_AUCTION_HOUSE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);

                SendGossipMenuFor(player, PREMIUM_MENU_TEXT, item->GetGUID());
                break;
            }
        }
    }

    void ApplyRandomMorph(Player* player)
    {
        uint32 random = (urand(1, 26)); // Change this line when adding more morphs
        {
            switch (random)
            {
                case 1: player->SetDisplayId(10134); break;     // Troll Female                 'Orb of Deception'
                case 2: player->SetDisplayId(10135); break;     // Troll Male                   'Orb of Deception'
                case 3: player->SetDisplayId(10136); break;     // Tauren Male                  'Orb of Deception'
                case 4: player->SetDisplayId(10137); break;     // Human Male                   'Orb of Deception'
                case 5: player->SetDisplayId(10138); break;     // Human Female                 'Orb of Deception'
                case 6: player->SetDisplayId(10139); break;     // Orc Male                     'Orb of Deception'
                case 7: player->SetDisplayId(10140); break;     // Orc Female                   'Orb of Deception'
                case 8: player->SetDisplayId(10141); break;     // Dwarf Male                   'Orb of Deception'
                case 9: player->SetDisplayId(10142); break;     // Dwarf Female                 'Orb of Deception'
                case 10: player->SetDisplayId(10143); break;    // NightElf Male                'Orb of Deception'
                case 11: player->SetDisplayId(10144); break;    // NightElf Female              'Orb of Deception'
                case 12: player->SetDisplayId(10145); break;    // Undead Female                'Orb of Deception'
                case 13: player->SetDisplayId(10146); break;    // Undead Male                  'Orb of Deception'
                case 14: player->SetDisplayId(10147); break;    // Tauren Female                'Orb of Deception'
                case 15: player->SetDisplayId(10148); break;    // Gnome Male                   'Orb of Deception'
                case 16: player->SetDisplayId(10149); break;    // Gnome Female                 'Orb of Deception'
                case 17: player->SetDisplayId(4527); break;     // Thrall                       'Orgrimmar Boss'
                case 18: player->SetDisplayId(11657); break;    // Lady Sylvanas                'Undercity Boss'
                case 19: player->SetDisplayId(4307); break;     // Cairne Bloodhoof             'Thunderbluff Boss'
                case 20: player->SetDisplayId(17122); break;    // Lor'themar Theron            'Silvermoon City Boss'
                case 21: player->SetDisplayId(3597); break;     // King Magni Bronzebeard       'Ironforge Boss'
                case 22: player->SetDisplayId(5566); break;     // Highlord Bolvar Fordragon    'Stormwind Boss'
                case 23: player->SetDisplayId(7006); break;     // High Tinker Mekkatorque      'Gnomer Boss'
                case 24: player->SetDisplayId(7274); break;     // Tyrande Whisperwind          'Darnassus Boss'
                case 25: player->SetDisplayId(21976); break;    // Arthus Small                 'Arthus'
                case 26: player->SetDisplayId(24641); break;    // Arthus Ghost                 'Arthus Ghost'

                default:
                    break;
            }
        }
    }

    void SummonTempNPC(Player* player, uint32 entry, const char* salute = "")
    {
        if (!player || entry == 0)
            return;

        int npcDuration = sConfigMgr->GetOption<int32>("Premium.NpcDuration", 60) * IN_MILLISECONDS;
        if (npcDuration <= 0) // Safeguard
            npcDuration = 60;

        Creature* npc = player->SummonCreature(entry, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, npcDuration);
        npc->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        npc->GetMotionMaster()->MoveFollow(player, PET_FOLLOW_DIST, player->GetFollowAngle());
        npc->SetFaction(player->GetFaction());

        if (salute && !(salute[0] == '\0'))
            npc->Whisper(salute, LANG_UNIVERSAL, player, false);
    }
};

void AddPremiumAccount()
{
    new premium_account();
}
