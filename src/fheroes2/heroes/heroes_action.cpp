/***************************************************************************
 *   Copyright (C) 2009 by Andrey Afletdinov <fheroes2@gmail.com>          *
 *                                                                         *
 *   Part of the Free Heroes2 Engine:                                      *
 *   http://sourceforge.net/projects/fheroes2                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "agg.h"
#include "mp2.h"
#include "world.h"
#include "settings.h"
#include "race.h"
#include "battle.h"
#include "game.h"
#include "game_interface.h"
#include "profit.h"
#include "maps_actions.h"
#include "ai.h"
#include "m82.h"
#include "mus.h"
#include "rand.h"
#include "audio_mixer.h"

#include "icn.h"
#include <functional>

#define PlayMusicReplacement(m82) if(MUS::FromMapObject(obj) == MUS::UNKNOWN) \
                                      AGG::PlaySound(m82)
#define PlaySoundWarning    PlayMusicReplacement(M82::EXPERNCE)
#define PlaySoundSuccess    PlayMusicReplacement(M82::TREASURE)
#define PlaySoundFailure    PlayMusicReplacement(M82::H2MINE)
#define PlaySoundVisited    PlayMusicReplacement(M82::RSBRYFZL)

void ActionToCastle(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToHeroes(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToMonster(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToBoat(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToCoast(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToWagon(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToSkeleton(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToObjectResource(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToPickupResource(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToFlotSam(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToArtifact(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToShipwreckSurvivor(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToShrine(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToWitchsHut(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToGoodLuckObject(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToPoorLuckObject(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToSign(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToMagicWell(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToTradingPost(Heroes& hero, uint32_t obj);

void ActionToPrimarySkillObject(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToPoorMoraleObject(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToGoodMoraleObject(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToExperienceObject(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToTreasureChest(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToAncientLamp(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToTeleports(Heroes& hero, s32 dst_index);

void ActionToWhirlpools(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToObservationTower(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToCaptureObject(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToDwellingJoinMonster(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToDwellingRecruitMonster(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToDwellingBattleMonster(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToArtesianSpring(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToAbandoneMine(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToXanadu(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToUpgradeArmyObject(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToMagellanMaps(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToEvent(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToObelisk(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToTreeKnowledge(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToOracle(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToDaemonCave(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToAlchemistsTower(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToStables(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToArena(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToSirens(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToJail(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToHutMagi(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToEyeMagi(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToSphinx(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToBarrier(Heroes& hero, uint32_t obj, s32 dst_index);

void ActionToTravellersTent(Heroes& hero, uint32_t obj, s32 dst_index);

uint32_t DialogCaptureResourceObject(const string& hdr, const string& str, uint32_t res, uint32_t buttons = Dialog::OK)
{
    const Sprite& sprite = AGG::GetICN(ICN::RESOURCE, Resource::GetIndexSprite2(res));
    string msg = str;

    // sprite resource with x / day test
    Surface sf(sprite.GetSize() + Size(30, 14), true);
    sprite.Blit(15, 0, sf);

    string perday = _("%{count} / day");
    payment_t info = ProfitConditions::FromMine(res);
    s32* current = nullptr;

    switch (res)
    {
    case Resource::MERCURY:
        current = &info.mercury;
        break;
    case Resource::WOOD:
        current = &info.wood;
        break;
    case Resource::ORE:
        current = &info.ore;
        break;
    case Resource::SULFUR:
        current = &info.sulfur;
        break;
    case Resource::CRYSTAL:
        current = &info.crystal;
        break;
    case Resource::GEMS:
        current = &info.gems;
        break;
    case Resource::GOLD:
        current = &info.gold;
        break;
    default:
        break;
    }

    if (current)
    {
        StringReplace(perday, "%{count}", *current);

        switch (*current)
        {
        case 1:
            StringReplace(msg, "%{count}", _("one"));
            break;
        case 2:
            StringReplace(msg, "%{count}", _("two"));
            break;
        default:
            StringReplace(msg, "%{count}", *current);
            break;
        }
    }

    Text text(perday, Font::SMALL);
    text.Blit((sf.w() - text.w()) / 2, sf.h() - 12, sf);

    return Dialog::SpriteInfo(hdr, msg, sf, buttons);
}

uint32_t DialogGoldWithExp(const string& hdr, const string& msg, uint32_t count, uint32_t exp,
                           uint32_t buttons = Dialog::OK)
{
    const Sprite& gold = AGG::GetICN(ICN::RESOURCE, 6);
    const Sprite& sprite = AGG::GetICN(ICN::EXPMRL, 4);
    Surface image(sprite.GetSize() + Size(gold.w() + 50, 12), true);
    gold.Blit(0, image.h() - gold.h() - 12, image);
    sprite.Blit(gold.w() + 50, 0, image);
    Text text(Int2Str(count));
    text.Blit((gold.w() - text.w()) / 2, image.h() - 12, image);
    text.Set(Int2Str(exp));
    text.Blit(gold.w() + 50 + (sprite.w() - text.w()) / 2, image.h() - 12, image);

    return Dialog::SpriteInfo(hdr, msg, image, buttons);
}

uint32_t DialogArtifactWithExp(const string& hdr, const string& msg, const Artifact& art, uint32_t exp,
                               uint32_t buttons = Dialog::OK)
{
    const Sprite& sprite = AGG::GetICN(ICN::EXPMRL, 4);
    const Sprite& border = AGG::GetICN(ICN::RESOURCE, 7);
    const Sprite& artifact = AGG::GetICN(ICN::ARTIFACT, art.IndexSprite64());
    Surface image(border.GetSize() + Size(sprite.w() + 50, 0), true);
    border.Blit(image);
    artifact.Blit(5, 5, image);
    sprite.Blit(border.w() + 50, (border.h() - sprite.h()) / 2, image);
    Text text(Int2Str(exp), Font::SMALL);
    text.Blit(border.w() + 50 + (sprite.w() - text.w()) / 2, image.h() - 12, image);

    return Dialog::SpriteInfo(hdr, msg, image, buttons);
}

uint32_t DialogWithExp(const string& hdr, const string& msg, uint32_t exp, uint32_t buttons = Dialog::OK)
{
    const Sprite& sprite = AGG::GetICN(ICN::EXPMRL, 4);
    Surface image(sprite.GetSize() + Size(0, 12), true);
    sprite.Blit(image);
    Text text(Int2Str(exp), Font::SMALL);
    text.Blit((sprite.w() - text.w()) / 2, sprite.h(), image);

    return Dialog::SpriteInfo(hdr, msg, image, buttons);
}

uint32_t DialogWithArtifactAndGold(const string& hdr, const string& msg, const Artifact& art, uint32_t count,
                                   uint32_t buttons = Dialog::OK)
{
    const Sprite& gold = AGG::GetICN(ICN::RESOURCE, 6);
    const Sprite& border = AGG::GetICN(ICN::RESOURCE, 7);
    const Sprite& artifact = AGG::GetICN(ICN::ARTIFACT, art.IndexSprite64());
    Surface image(border.GetSize() + Size(gold.w() + 50, 0), true);
    border.Blit(image);
    artifact.Blit(5, 5, image);
    gold.Blit(border.w() + 50, (border.h() - gold.h()) / 2, image);
    Text text(Int2Str(count), Font::SMALL);
    text.Blit(border.w() + 50 + (gold.w() - text.w()) / 2, border.h() - 25, image);

    return Dialog::SpriteInfo(hdr, msg, image, buttons);
}

uint32_t DialogWithGold(const string& hdr, const string& msg, uint32_t count, uint32_t buttons = Dialog::OK)
{
    const Sprite& gold = AGG::GetICN(ICN::RESOURCE, 6);
    Surface image(gold.GetSize() + Size(0, 12), true);
    gold.Blit(image);
    Text text(Int2Str(count), Font::SMALL);
    text.Blit((gold.w() - text.w()) / 2, gold.h(), image);

    return Dialog::SpriteInfo(hdr, msg, image, buttons);
}

uint32_t DialogMorale(const string& hdr, const string& msg, bool good, uint32_t count)
{
    if (1 > count) count = 1;
    if (3 < count) count = 3;
    const Sprite& sprite = AGG::GetICN(ICN::EXPMRL, good ? 2 : 3);
    uint32_t offset = sprite.w() * 4 / 3;
    Surface image(Size(sprite.w() + offset * (count - 1), sprite.h()), true);
    for (uint32_t ii = 0; ii < count; ++ii) sprite.Blit(offset * ii, 0, image);

    return Dialog::SpriteInfo(hdr, msg, image);
}

uint32_t DialogLuck(const string& hdr, const string& msg, bool good, uint32_t count)
{
    if (1 > count) count = 1;
    if (3 < count) count = 3;
    const Sprite& sprite = AGG::GetICN(ICN::EXPMRL, good ? 0 : 1);
    uint32_t offset = sprite.w() * 4 / 3;
    Surface image(Size(sprite.w() + offset * (count - 1), sprite.h()), true);
    for (uint32_t ii = 0; ii < count; ++ii) sprite.Blit(offset * ii, 0, image);

    return Dialog::SpriteInfo(hdr, msg, image);
}

void BattleLose(Heroes& hero, const Battle::Result& res, bool attacker, int color = Color::NONE)
{
    uint32_t reason = attacker ? res.AttackerResult() : res.DefenderResult();

    if (Settings::Get().ExtHeroSurrenderingGiveExp() &&
        Battle::RESULT_SURRENDER == reason)
    {
        const uint32_t& exp = attacker ? res.GetExperienceAttacker() : res.GetExperienceDefender();

        if (hero.isControlHuman())
        {
            string msg = _("Hero %{name} also got a %{count} experience.");
            StringReplace(msg, "%{name}", hero.GetName());
            StringReplace(msg, "%{count}", exp);
            Message("", msg, Font::BIG, Dialog::OK);
        }
        hero.IncreaseExperience(exp);
    }

    AGG::PlaySound(M82::KILLFADE);
    hero.FadeOut();
    hero.SetKillerColor(color);
    hero.SetFreeman(reason);

    Interface::Basic& I = Interface::Basic::Get();
    I.ResetFocus(GameFocus::HEROES);
    I.RedrawFocus();
}

void AnimationRemoveObject(Maps::Tiles& tile)
{
    Maps::TilesAddon* addon = MP2::isRemoveObject(tile.GetObject()) ? tile.FindObject(tile.GetObject()) : nullptr;

    if (nullptr == addon) return;

    const Interface::GameArea& gamearea = Interface::Basic::Get().GetGameArea();
    const Point& area = gamearea.GetMapsPos();
    const Rect& rect = gamearea.GetRectMaps();
    const Point pos = Maps::GetPoint(tile.GetIndex()) - rect;

    const s32 dstx = area.x + TILEWIDTH * pos.x;
    const s32 dsty = area.y + TILEWIDTH * pos.y;

    Cursor& cursor = Cursor::Get();
    Display& display = Display::Get();

    MapsIndexes heroes;
    Maps::ScanAroundObject(tile.GetIndex(), MP2::OBJ_HEROES, heroes);
    const Surface& stile = tile.GetTileSurface();
    Surface sobj(stile.GetSize(), true);

    const Sprite& sprite = AGG::GetICN(MP2::GetICNObject(addon->object), addon->index);
    sprite.Blit(sprite.x(), sprite.y(), sobj);

    // if animation sprite
    uint32_t index;
    if (0 != (index = ICN::AnimationFrame(MP2::GetICNObject(addon->object), addon->index, 0)))
    {
        const Sprite& anim1 = AGG::GetICN(MP2::GetICNObject(addon->object), index);
        anim1.Blit(anim1.x(), anim1.y(), sobj);
    }

    LocalEvent& le = LocalEvent::Get();
    uint32_t alpha = 250;

    while (le.HandleEvents() && alpha > 10)
    {
        if (AnimateInfrequentDelay(Game::HEROES_PICKUP_DELAY))
        {
            cursor.Hide();
            stile.Blit(dstx, dsty, display);
            sobj.SetAlphaMod(alpha);
            sobj.Blit(dstx, dsty, display);

            if (!heroes.empty())
            {
                for (MapsIndexes::const_iterator
                     it = heroes.begin(); it != heroes.end(); ++it)
                {
                    Heroes* hero = world.GetTiles(*it).GetHeroes();
                    if (hero) hero->Redraw(display, false);
                }
            }
            else
                tile.RedrawTop(display);
            cursor.Show();
            display.Flip();
            alpha -= 20;
        }
    }
}

void RecruitMonsterFromTile(Heroes& hero, Maps::Tiles& tile, const string& msg, const Troop& troop, bool remove)
{
    if (!hero.GetArmy().m_troops.CanJoinTroop(troop._monster))
        Message(msg, _("You are unable to recruit at this time, your ranks are full."), Font::BIG, Dialog::OK);
    else
    {
        const uint32_t recruit = Dialog::RecruitMonster(troop(), troop.GetCount(), false).GetCount();

        if (recruit)
        {
            if (remove && recruit == troop.GetCount())
            {
                Game::PlayPickupSound();
                AnimationRemoveObject(tile);
                tile.MonsterSetCount(0);
                tile.RemoveObjectSprite();
                tile.SetObject(MP2::OBJ_ZERO);
            }
            else
                tile.MonsterSetCount(troop.GetCount() - recruit);

            const payment_t paymentCosts = troop().GetCost() * recruit;
            hero.GetKingdom().OddFundsResource(paymentCosts);

            hero.GetArmy().m_troops.JoinTroop(troop(), recruit);
            hero.MovePointsScaleFixed();

            if (Settings::Get().ExtHeroRecalculateMovement())
                hero.RecalculateMovePoints();

            Interface::Basic::Get().GetStatusWindow().SetRedraw();
        }
    }
}

// action to next cell
void Heroes::Action(s32 dst_index)
{
    if (GetKingdom().isControlAI())
        return AI::HeroesAction(*this, dst_index);

    const Maps::Tiles& tile = world.GetTiles(dst_index);
    const int object = dst_index == GetIndex() ? tile.GetObject(false) : tile.GetObject();

    if (MUS::FromMapObject(object) != MUS::UNKNOWN)
        AGG::PlayMusic(MUS::FromMapObject(object), false);

    if (MP2::isActionObject(object, isShipMaster()))
    {
        Interface::Basic::Get().GetStatusWindow().ResetTimer();
        SetModes(ACTION);
    }

    /* new format map only */
    ListActions* listActions = world.GetListActions(dst_index);
    bool cancel_default = false;

    if (listActions)
    {
        for (const auto& it : listActions->_items)
        {
            switch (it->GetType())
            {
            case ACTION_ACCESS:
                if (!ActionAccess::Action(static_cast<ActionAccess *>(it.get()), dst_index, *this))
                    cancel_default = true;
                break;

            case ACTION_DEFAULT:
                if (!ActionDefault::Action(static_cast<ActionDefault *>(it.get()), dst_index, *this))
                    cancel_default = true;
                break;

            case ACTION_MESSAGE:
                ActionMessage::Action(static_cast<ActionMessage *>(it.get()), dst_index, *this);
                break;

            case ACTION_RESOURCES:
                ActionResources::Action(static_cast<ActionResources *>(it.get()), dst_index, *this);
                break;

            case ACTION_ARTIFACT:
                ActionArtifact::Action(static_cast<ActionArtifact *>(it.get()), dst_index, *this);
                break;

            default:
                break;
            }
        }
    }

    /* default actions */
    if (cancel_default)
    {
        if (MP2::isPickupObject(object))
        {
            Maps::Tiles& tile = world.GetTiles(dst_index);
            AnimationRemoveObject(tile);
            tile.RemoveObjectSprite();
            tile.QuantityReset();
            tile.SetObject(MP2::OBJ_ZERO);
        }
    }
    else
        switch (object)
        {
        case MP2::OBJ_MONSTER:
            ActionToMonster(*this, object, dst_index);
            break;

        case MP2::OBJ_CASTLE:
            ActionToCastle(*this, object, dst_index);
            break;
        case MP2::OBJ_HEROES:
            ActionToHeroes(*this, object, dst_index);
            break;

        case MP2::OBJ_BOAT:
            ActionToBoat(*this, object, dst_index);
            break;
        case MP2::OBJ_COAST:
            ActionToCoast(*this, object, dst_index);
            break;

            // resource object
        case MP2::OBJ_WINDMILL:
        case MP2::OBJ_WATERWHEEL:
        case MP2::OBJ_MAGICGARDEN:
        case MP2::OBJ_LEANTO:
            ActionToObjectResource(*this, object, dst_index);
            break;

        case MP2::OBJ_WAGON:
            ActionToWagon(*this, object, dst_index);
            break;
        case MP2::OBJ_SKELETON:
            ActionToSkeleton(*this, object, dst_index);
            break;

            // pickup object
        case MP2::OBJ_RESOURCE:
        case MP2::OBJ_BOTTLE:
        case MP2::OBJ_CAMPFIRE:
            ActionToPickupResource(*this, object, dst_index);
            break;

        case MP2::OBJ_WATERCHEST:
        case MP2::OBJ_TREASURECHEST:
            ActionToTreasureChest(*this, object, dst_index);
            break;
        case MP2::OBJ_ANCIENTLAMP:
            ActionToAncientLamp(*this, object, dst_index);
            break;
        case MP2::OBJ_FLOTSAM:
            ActionToFlotSam(*this, object, dst_index);
            break;

        case MP2::OBJ_SHIPWRECKSURVIROR:
            ActionToShipwreckSurvivor(*this, object, dst_index);
            break;
        case MP2::OBJ_ARTIFACT:
            ActionToArtifact(*this, object, dst_index);
            break;

            // shrine circle
        case MP2::OBJ_SHRINE1:
        case MP2::OBJ_SHRINE2:
        case MP2::OBJ_SHRINE3:
            ActionToShrine(*this, object, dst_index);
            break;

            // witchs hut
        case MP2::OBJ_WITCHSHUT:
            ActionToWitchsHut(*this, object, dst_index);
            break;

            // info message
        case MP2::OBJ_SIGN:
            ActionToSign(*this, object, dst_index);
            break;

            // luck modification
        case MP2::OBJ_FOUNTAIN:
        case MP2::OBJ_FAERIERING:
        case MP2::OBJ_IDOL:
            ActionToGoodLuckObject(*this, object, dst_index);
            break;

        case MP2::OBJ_PYRAMID:
            ActionToPoorLuckObject(*this, object, dst_index);
            break;
        case MP2::OBJ_MAGICWELL:
            ActionToMagicWell(*this, object, dst_index);
            break;
        case MP2::OBJ_TRADINGPOST:
            ActionToTradingPost(*this, object);
            break;

            // primary skill modification
        case MP2::OBJ_FORT:
        case MP2::OBJ_MERCENARYCAMP:
        case MP2::OBJ_DOCTORHUT:
        case MP2::OBJ_STANDINGSTONES:
            ActionToPrimarySkillObject(*this, object, dst_index);
            break;

            // morale modification
        case MP2::OBJ_OASIS:
        case MP2::OBJ_TEMPLE:
        case MP2::OBJ_WATERINGHOLE:
        case MP2::OBJ_BUOY:
            ActionToGoodMoraleObject(*this, object, dst_index);
            break;

        case MP2::OBJ_SHIPWRECK:
        case MP2::OBJ_GRAVEYARD:
        case MP2::OBJ_DERELICTSHIP:
            ActionToPoorMoraleObject(*this, object, dst_index);
            break;

            // experience modification
        case MP2::OBJ_GAZEBO:
            ActionToExperienceObject(*this, object, dst_index);
            break;
        case MP2::OBJ_DAEMONCAVE:
            ActionToDaemonCave(*this, object, dst_index);
            break;

            // teleports
        case MP2::OBJ_STONELIGHTS:
            ActionToTeleports(*this, dst_index);
            break;
        case MP2::OBJ_WHIRLPOOL:
            ActionToWhirlpools(*this, object, dst_index);
            break;

            // obsv tower
        case MP2::OBJ_OBSERVATIONTOWER:
            ActionToObservationTower(*this, object, dst_index);
            break;
        case MP2::OBJ_MAGELLANMAPS:
            ActionToMagellanMaps(*this, object, dst_index);
            break;

            // capture color object
        case MP2::OBJ_ALCHEMYLAB:
        case MP2::OBJ_MINES:
        case MP2::OBJ_SAWMILL:
        case MP2::OBJ_LIGHTHOUSE:
            ActionToCaptureObject(*this, object, dst_index);
            break;

        case MP2::OBJ_ABANDONEDMINE:
            ActionToAbandoneMine(*this, object, dst_index);
            break;

            // accept army
        case MP2::OBJ_WATCHTOWER:
        case MP2::OBJ_EXCAVATION:
        case MP2::OBJ_CAVE:
        case MP2::OBJ_TREEHOUSE:
        case MP2::OBJ_ARCHERHOUSE:
        case MP2::OBJ_GOBLINHUT:
        case MP2::OBJ_DWARFCOTT:
        case MP2::OBJ_HALFLINGHOLE:
        case MP2::OBJ_PEASANTHUT:
        case MP2::OBJ_THATCHEDHUT:
            ActionToDwellingJoinMonster(*this, object, dst_index);
            break;

            // recruit army
        case MP2::OBJ_RUINS:
        case MP2::OBJ_TREECITY:
        case MP2::OBJ_WAGONCAMP:
        case MP2::OBJ_DESERTTENT:
            ActionToDwellingRecruitMonster(*this, object, dst_index);
            break;

            // battle and recruit army
        case MP2::OBJ_DRAGONCITY:
        case MP2::OBJ_CITYDEAD:
        case MP2::OBJ_TROLLBRIDGE:
            ActionToDwellingBattleMonster(*this, object, dst_index);
            break;

        case MP2::OBJ_ARTESIANSPRING:
            ActionToArtesianSpring(*this, object, dst_index);
            break;

        case MP2::OBJ_XANADU:
            ActionToXanadu(*this, object, dst_index);
            break;

        case MP2::OBJ_HILLFORT:
        case MP2::OBJ_FREEMANFOUNDRY:
            ActionToUpgradeArmyObject(*this, object, dst_index);
            break;

        case MP2::OBJ_EVENT:
            ActionToEvent(*this, object, dst_index);
            break;

        case MP2::OBJ_OBELISK:
            ActionToObelisk(*this, object, dst_index);
            break;

        case MP2::OBJ_TREEKNOWLEDGE:
            ActionToTreeKnowledge(*this, object, dst_index);
            break;

        case MP2::OBJ_ORACLE:
            ActionToOracle(*this, object, dst_index);
            break;
        case MP2::OBJ_SPHINX:
            ActionToSphinx(*this, object, dst_index);
            break;

            // loyalty version
        case MP2::OBJ_WATERALTAR:
        case MP2::OBJ_AIRALTAR:
        case MP2::OBJ_FIREALTAR:
        case MP2::OBJ_EARTHALTAR:
        case MP2::OBJ_BARROWMOUNDS:
            ActionToDwellingRecruitMonster(*this, object, dst_index);
            break;
        case MP2::OBJ_ALCHEMYTOWER:
            ActionToAlchemistsTower(*this, object, dst_index);
            break;
        case MP2::OBJ_STABLES:
            ActionToStables(*this, object, dst_index);
            break;
        case MP2::OBJ_ARENA:
            ActionToArena(*this, object, dst_index);
            break;
        case MP2::OBJ_MERMAID:
            ActionToGoodLuckObject(*this, object, dst_index);
            break;
        case MP2::OBJ_SIRENS:
            ActionToSirens(*this, object, dst_index);
            break;
        case MP2::OBJ_JAIL:
            ActionToJail(*this, object, dst_index);
            break;
        case MP2::OBJ_HUTMAGI:
            ActionToHutMagi(*this, object, dst_index);
            break;
        case MP2::OBJ_EYEMAGI:
            ActionToEyeMagi(*this, object, dst_index);
            break;

        case MP2::OBJ_BARRIER:
            ActionToBarrier(*this, object, dst_index);
            break;
        case MP2::OBJ_TRAVELLERTENT:
            ActionToTravellersTent(*this, object, dst_index);
            break;

            // object
        default:
            break;
        }
}

Battle::Result BattleHeroWithMonster(Heroes& hero, Army& army, s32 dst_index)
{
    Battle::Result res;
    std::vector<u8> armyVec;
    std::vector<u8> heroVec;
    {
        ByteVectorWriter armyBuf, heroArmyBuf;
        armyBuf << army;
        heroArmyBuf << hero;
        armyVec = armyBuf.data();
        heroVec = heroArmyBuf.data();
    }

    Settings::Get().SetQuickCombat(true);
    do
    {
        res = Battle::Loader(hero.GetArmy(), army, dst_index);
        if (res.fightAgain == Battle::FightResultType::FightAgain)
        {
            Settings::Get().SetQuickCombat(false);
            ByteVectorReader bvr(armyVec);
            bvr >> army;
            ByteVectorReader heroArmyReader(heroVec);
            heroArmyReader >> hero;
        }
    }
    while (res.fightAgain == Battle::FightResultType::FightAgain);
    Settings::Get().SetQuickCombat(true);
    return res;
}


Battle::Result BattleHeroWithHero(Heroes& hero, Heroes& other_hero, s32 dst_index)
{
    Battle::Result res;
    std::vector<u8> armyVec, heroVec;
    {
        ByteVectorWriter armyBuf, heroArmyBuf;
        armyBuf << other_hero;
        armyVec = armyBuf.data();
        heroArmyBuf << hero;
        heroVec = heroArmyBuf.data();
    }
    Settings::Get().SetQuickCombat(true);
    do
    {
        res = Battle::Loader(hero.GetArmy(), other_hero.GetArmy(), dst_index);
        if (res.fightAgain == Battle::FightResultType::FightAgain)
        {
            Settings::Get().SetQuickCombat(false);
            ByteVectorReader bvr(armyVec);
            bvr >> other_hero;
            ByteVectorReader heroArmyReader(heroVec);
            heroArmyReader >> hero;
        }
    }
    while (res.fightAgain == Battle::FightResultType::FightAgain);
    Settings::Get().SetQuickCombat(true);
    return res;
}

void ActionToMonster(Heroes& hero, uint32_t obj, s32 dst_index)
{
    bool destroy = false;
    Maps::Tiles& tile = world.GetTiles(dst_index);
    auto map_troop = static_cast<MapMonster *>(world.GetMapObject(tile.GetObjectUID(obj)));
    Troop troop = map_troop ? map_troop->QuantityTroop() : tile.QuantityTroop();
    //const Settings & conf = Settings::Get();

    JoinCount join = Army::GetJoinSolution(hero, tile, troop);

    Interface::StatusWindow& statusWindow = Interface::Basic::Get().GetStatusWindow();

    // free join
    if (JOIN_FREE == join.first)
    {
        if (Dialog::YES == Dialog::ArmyJoinFree(troop, hero))
        {
            hero.GetArmy().m_troops.JoinTroop(troop);
            statusWindow.SetRedraw();
        }
        else
        {
            Message("", _("Insulted by your refusal of their offer, the monsters attack!"), Font::BIG,
                    Dialog::OK);
            join.first = JOIN_NONE;
        }
    }
    else
        // join with cost
        if (JOIN_COST == join.first)
        {
            uint32_t gold = troop.GetCost().gold;

            if (Dialog::YES == Dialog::ArmyJoinWithCost(troop, join.second, gold, hero))
            {
                hero.GetArmy().m_troops.JoinTroop(troop(), join.second);
                hero.GetKingdom().OddFundsResource(Funds(Resource::GOLD, gold));
                statusWindow.SetRedraw();
            }
            else
            {
                Message("", _("Insulted by your refusal of their offer, the monsters attack!"), Font::BIG,
                        Dialog::OK);
                join.first = JOIN_NONE;
            }
        }
        else
            // flee
            if (JOIN_FLEE == join.first)
            {
                string message = _(
                    "The %{monster}, awed by the power of your forces, begin to scatter.\nDo you wish to pursue and engage them?"
                );
                StringReplace(message, "%{monster}", StringLower(troop._monster.GetMultiName()));

                if (Dialog::Message("", message, Font::BIG, Dialog::YES | Dialog::NO) == Dialog::NO)
                    destroy = true;
                else
                    join.first = 0;
            }

    //bool allow_move = false;

    // fight
    if (JOIN_NONE == join.first)
    {
        Army army(tile);
        Battle::Result res = BattleHeroWithMonster(hero, army, dst_index);

        if (res.AttackerWins())
        {
            hero.IncreaseExperience(res.GetExperienceAttacker());
            destroy = true;
            //allow_move = true;
        }
        else
        {
            BattleLose(hero, res, true);
            if (Settings::Get().ExtWorldSaveMonsterBattle())
            {
                tile.MonsterSetCount(army.m_troops.GetCountMonsters(troop()));
                // reset "can join"
                if (tile.MonsterJoinConditionFree()) tile.MonsterSetJoinCondition(Monster::JOIN_CONDITION_MONEY);

                if (map_troop)
                {
                    map_troop->count = army.m_troops.GetCountMonsters(troop());
                    if (map_troop->JoinConditionFree()) map_troop->condition = Monster::JOIN_CONDITION_MONEY;
                }
            }
        }
    }
        // unknown
    else
        destroy = true;

    if (destroy)
    {
        Maps::TilesAddon* addon = tile.FindObject(MP2::OBJ_MONSTER);
        if (addon)
        {
            AGG::PlaySound(M82::KILLFADE);
            const uint32_t uniq = addon->uniq;
            AnimationRemoveObject(tile);
            tile.Remove(uniq);
            tile.MonsterSetCount(0);
            tile.SetObject(MP2::OBJ_ZERO);

            Size wSize(world.w(), world.h());
            // remove shadow from left cell
            if (Maps::isValidDirection(dst_index, Direction::LEFT, wSize))
                world.GetTiles(Maps::GetDirectionIndex(dst_index, Direction::LEFT)).Remove(uniq);
        }

        if (map_troop) world.RemoveMapObject(map_troop);

        // auto move hero
        // disable: https://sourceforge.net/tracker/index.php?func=detail&aid=3155230&group_id=96859&atid=616180
        /*
        if(conf.ExtHeroAutoMove2BattleTarget() && allow_move)
        {
            hero.Move2Dest(dst_index);
        }
        */
    }
}

void ActionToHeroes(Heroes& hero, uint32_t obj, s32 dst_index)
{
    Heroes* other_hero = world.GetTiles(dst_index).GetHeroes();
    const Settings& conf = Settings::Get();

    if (!other_hero) return;

    if (hero.GetColor() == other_hero->GetColor() ||
        (conf.ExtUnionsAllowHeroesMeetings() && Players::isFriends(hero.GetColor(), other_hero->GetColor())))
    {
        hero.MeetingDialog(*other_hero);
    }
    else if (hero.isFriends(other_hero->GetColor()))
    {
    }
    else if (!hero.AllowBattle(true))
    {
    }
    else if (!other_hero->AllowBattle(false))
    {
    }
    else
    {
        Castle* other_hero_castle = other_hero->inCastle();
        if (other_hero_castle && other_hero == other_hero_castle->GetHeroes().GuardFirst())
        {
            ActionToCastle(hero, MP2::OBJ_CASTLE, dst_index);
            return;
        }

        bool disable_auto_move = hero.isShipMaster() || other_hero->isShipMaster() ||
            other_hero_castle ||
            world.GetTiles(hero.GetIndex()).GetObject(false) == MP2::OBJ_STONELIGHTS;

        // new battle
        Battle::Result res = BattleHeroWithHero(hero, *other_hero, dst_index);

        // loss defender
        if (!res.DefenderWins())
            BattleLose(*other_hero, res, false, hero.GetColor());

        // loss attacker
        if (!res.AttackerWins())
            BattleLose(hero, res, true, other_hero->GetColor());

        // wins attacker
        if (res.AttackerWins())
        {
            hero.IncreaseExperience(res.GetExperienceAttacker());

            // auto move hero
            if (conf.ExtHeroAutoMove2BattleTarget() && !disable_auto_move)
            {
                hero.Move2Dest(dst_index);
            }
        }
        else
            // wins defender
            if (res.DefenderWins())
            {
                other_hero->IncreaseExperience(res.GetExperienceDefender());
            }
    }
}

void ActionToCastle(Heroes& hero, uint32_t obj, s32 dst_index)
{
    Castle* castle = world.GetCastle(Maps::GetPoint(dst_index));
    const Settings& conf = Settings::Get();

    if (!castle)
    {
    }
    else if (hero.GetColor() == castle->GetColor() ||
        (conf.ExtUnionsAllowCastleVisiting() && Players::isFriends(hero.GetColor(), castle->GetColor())))
    {
        Mixer::Reduce();
        if (!conf.ExtHeroLearnSpellsWithDay()) castle->MageGuildEducateHero(hero);
        Game::OpenCastleDialog(*castle);
        Mixer::Enhance();
    }
    else if (hero.isFriends(castle->GetColor()))
    {
    }
    else
    {
        CastleHeroes heroes = castle->GetHeroes();

        // first attack to guest hero
        if (heroes.FullHouse())
        {
            ActionToHeroes(hero, MP2::OBJ_HEROES, dst_index);
            return;
        }

        Army& army = castle->GetActualArmy();
        bool allow_enter = false;

        if (army.m_troops.isValid())
        {
            Heroes* defender = heroes.GuardFirst();
            castle->ActionPreBattle();

            // new battle
            Battle::Result res = BattleHeroWithMonster(hero, army, dst_index);

            castle->ActionAfterBattle(res.AttackerWins());

            // loss defender
            if (!res.DefenderWins() && defender)
                BattleLose(*defender, res, false, hero.GetColor());

            // loss attacker
            if (!res.AttackerWins())
                BattleLose(hero, res, true, castle->GetColor());

            // wins attacker
            if (res.AttackerWins())
            {
                castle->GetKingdom().RemoveCastle(castle);
                hero.GetKingdom().AddCastle(castle);
                world.CaptureObject(dst_index, hero.GetColor());
                castle->Scoute();
                Interface::Basic::Get().SetRedraw(REDRAW_CASTLES);

                hero.IncreaseExperience(res.GetExperienceAttacker());
                allow_enter = true;
            }
            else
                // wins defender
                if (res.DefenderWins() && defender)
                {
                    defender->IncreaseExperience(res.GetExperienceDefender());
                }
        }
        else
        {
            castle->GetKingdom().RemoveCastle(castle);
            hero.GetKingdom().AddCastle(castle);
            world.CaptureObject(dst_index, hero.GetColor());
            castle->Scoute();
            Interface::Basic::Get().SetRedraw(REDRAW_CASTLES);
            allow_enter = true;
        }

        // auto move hero to castle
        if (conf.ExtHeroAutoMove2BattleTarget() && allow_enter)
        {
            hero.Move2Dest(dst_index);
            ActionToCastle(hero, MP2::OBJ_CASTLE, dst_index);
        }
    }
}

void ActionToBoat(Heroes& hero, uint32_t obj, s32 dst_index)
{
    if (hero.isShipMaster()) return;

    AGG::PlaySound(M82::KILLFADE);
    hero.GetPath().Hide();
    hero.FadeOut();
    hero.ApplyPenaltyMovement();
    hero.ApplyPenaltyMovement();

    hero.Move2Dest(dst_index);
    hero.SetMapsObject(MP2::OBJ_ZERO);
    hero.SetShipMaster(true);
    hero.GetPath().Reset();
}

void ActionToCoast(Heroes& hero, uint32_t obj, s32 dst_index)
{
    if (!hero.isShipMaster()) return;

    Maps::Tiles& from = world.GetTiles(hero.GetIndex());

    hero.ApplyPenaltyMovement();
    hero.ApplyPenaltyMovement();

    hero.Move2Dest(dst_index);
    from.SetObject(MP2::OBJ_BOAT);
    hero.SetShipMaster(false);
    AGG::PlaySound(M82::KILLFADE);
    hero.GetPath().Hide();
    hero.FadeIn();
    hero.GetPath().Reset();
}

void ActionToPickupResource(Heroes& hero, uint32_t obj, s32 dst_index)
{
    Maps::Tiles& tile = world.GetTiles(dst_index);
    auto* map_resource = static_cast<MapResource *>(world.GetMapObject(tile.GetObjectUID(obj)));

    Game::PlayPickupSound();
    AnimationRemoveObject(tile);
    tile.RemoveObjectSprite();

    if (obj == MP2::OBJ_BOTTLE)
    {
        auto sign = static_cast<MapSign *>(world.GetMapObject(tile.GetObjectUID(obj)));
        Message(MP2::StringObject(obj), sign ? sign->message : "", Font::BIG, Dialog::OK);
    }
    else
    {
        Funds funds = map_resource ? Funds(map_resource->resource) : tile.QuantityFunds();

        if (obj == MP2::OBJ_CAMPFIRE)
        {
            // force reset sound
            tile.SetObject(MP2::OBJ_ZERO);
            Game::EnvironmentSoundMixer();

            Dialog::ResourceInfo(MP2::StringObject(obj),
                                 _("Ransacking an enemy camp, you discover a hidden cache of treasures."), funds);
        }
        else
        {
            ResourceCount rc = tile.QuantityResourceCount();
            if (map_resource) rc = map_resource->resource;

            Interface::Basic& I = Interface::Basic::Get();
            I.GetStatusWindow().SetResource(rc.first, rc.second);
            I.SetRedraw(REDRAW_STATUS);
        }

        hero.GetKingdom().AddFundsResource(funds);
    }

    tile.QuantityReset();
    if (map_resource) world.RemoveMapObject(map_resource);
}

void ActionToObjectResource(Heroes& hero, uint32_t obj, s32 dst_index)
{
    Maps::Tiles& tile = world.GetTiles(dst_index);
    ResourceCount rc = tile.QuantityResourceCount();
    bool cancapture = Settings::Get().ExtWorldExtObjectsCaptured();
    bool showinvalid = !(cancapture && hero.GetColor() == tile.QuantityColor());

    string msg;

    // dialog
    switch (obj)
    {
    case MP2::OBJ_WINDMILL:
        msg = rc.isValid()
                  ? _(
                      "The keeper of the mill announces:\n\"Milord, I have been working very hard to provide you with these resources, come back next week for more.\""
                  )
                  : _(
                      "The keeper of the mill announces:\n\"Milord, I am sorry, there are no resources currently available. Please try again next week.\""
                  );
        break;

    case MP2::OBJ_WATERWHEEL:
        msg = rc.isValid()
                  ? _(
                      "The keeper of the mill announces:\n\"Milord, I have been working very hard to provide you with this gold, come back next week for more.\""
                  )
                  : _(
                      "The keeper of the mill announces:\n\"Milord, I am sorry, there is no gold currently available. Please try again next week.\""
                  );
        break;

    case MP2::OBJ_LEANTO:
        cancapture = false;
        msg = rc.isValid()
                  ? _("You've found an abandoned lean-to.\nPoking about, you discover some resources hidden nearby.")
                  : _("The lean-to is long abandoned. There is nothing of value here.");
        break;

    case MP2::OBJ_MAGICGARDEN:
        msg = rc.isValid()
                  ? _(
                      "You catch a leprechaun foolishly sleeping amidst a cluster of magic mushrooms.\nIn exchange for his freedom, he guides you to a small pot filled with precious things."
                  )
                  : _(
                      "You've found a magic garden, the kind of place that leprechauns and faeries like to cavort in, but there is no one here today.\nPerhaps you should try again next week."
                  );
        break;

    default:
        cancapture = false;
        break;
    }

    if (rc.isValid())
    {
        const Funds funds(rc);
        PlaySoundSuccess;
        Dialog::ResourceInfo("", msg, funds);
        hero.GetKingdom().AddFundsResource(funds);

        if (cancapture)
            ActionToCaptureObject(hero, obj, dst_index);
    }
    else
    {
        if (cancapture)
            ActionToCaptureObject(hero, obj, dst_index);

        if (showinvalid)
            Message("", msg, Font::BIG, Dialog::OK);
    }

    tile.QuantityReset();
    hero.SetVisited(dst_index, Visit::GLOBAL);
}

void ActionToSkeleton(Heroes& hero, uint32_t obj, s32 dst_index)
{
    Maps::Tiles& tile = world.GetTiles(dst_index);
    string message(_("You come upon the remains of an unfortunate adventurer."));

    // artifact
    if (tile.QuantityIsValid())
    {
        Game::PlayPickupSound();

        if (hero.IsFullBagArtifacts())
        {
            uint32_t gold = GoldInsteadArtifact(obj);
            const Funds funds(Resource::GOLD, gold);
            ResourceInfo("", _("Treasure"), funds, Dialog::OK);
            hero.GetKingdom().AddFundsResource(funds);
        }
        else
        {
            const Artifact& art = tile.QuantityArtifact();
            message.append("\n");
            message.append(_("Searching through the tattered clothing, you find %{artifact}."));
            StringReplace(message, "%{artifact}", art.GetName());
            Dialog::ArtifactInfo("", message, art);
            hero.PickupArtifact(art);
        }

        tile.QuantityReset();
    }
    else
    {
        message.append("\n");
        message.append(_("Searching through the tattered clothing, you find nothing."));
        Message("", message, Font::BIG, Dialog::OK);
    }

    hero.SetVisitedWideTile(dst_index, obj, Visit::GLOBAL);
}

void ActionToWagon(Heroes& hero, uint32_t obj, s32 dst_index)
{
    Maps::Tiles& tile = world.GetTiles(dst_index);
    string message(_("You come across an old wagon left by a trader who didn't quite make it to safe terrain."));

    if (tile.QuantityIsValid())
    {
        const Artifact& art = tile.QuantityArtifact();

        if (art.isValid())
        {
            if (hero.IsFullBagArtifacts())
            {
                message.append("\n");
                message.append(_("Unfortunately, others have found it first, and the wagon is empty."));
                Message("", message, Font::BIG, Dialog::OK);
            }
            else
            {
                Game::PlayPickupSound();
                message.append("\n");
                message.append(_("Searching inside, you find the %{artifact}."));
                StringReplace(message, "%{artifact}", art.GetName());
                Dialog::ArtifactInfo("", message, art);
                hero.PickupArtifact(art);
            }
        }
        else
        {
            const Funds& funds = tile.QuantityFunds();
            Game::PlayPickupSound();
            message.append("\n");
            message.append(_("Inside, you find some of the wagon's cargo still intact."));
            Dialog::ResourceInfo("", message, funds);
            hero.GetKingdom().AddFundsResource(funds);
        }

        tile.QuantityReset();
    }
    else
    {
        message.append("\n");
        message.append(_("Unfortunately, others have found it first, and the wagon is empty."));
        Message("", message, Font::BIG, Dialog::OK);
    }

    hero.SetVisited(dst_index, Visit::GLOBAL);
}

void ActionToFlotSam(Heroes& hero, uint32_t obj, s32 dst_index)
{
    Maps::Tiles& tile = world.GetTiles(dst_index);
    string msg;
    const Funds& funds = tile.QuantityFunds();

    Game::PlayPickupSound();
    AnimationRemoveObject(tile);

    if (0 < funds.GetValidItemsCount())
    {
        msg = funds.wood && funds.gold
                  ? _("You search through the flotsam, and find some wood and some gold.")
                  : _("You search through the flotsam, and find some wood.");
        Dialog::ResourceInfo(MP2::StringObject(obj), msg, funds);
        hero.GetKingdom().AddFundsResource(funds);
    }
    else
    {
        msg = _("You search through the flotsam, but find nothing.");
        Message(MP2::StringObject(obj), msg, Font::BIG, Dialog::OK);
    }

    tile.RemoveObjectSprite();
    tile.QuantityReset();
}

void ActionToShrine(Heroes& hero, uint32_t obj, s32 dst_index)
{
    const Spell& spell = world.GetTiles(dst_index).QuantitySpell();
    const uint32_t spell_level = spell.Level();

    string head;
    string body;

    switch (spell_level)
    {
    case 1:
        head = _("Shrine of the 1st Circle");
        body = _(
            "You come across a small shrine attended by a group of novice acolytes.\nIn exchange for your protection, they agree to teach you a simple spell - '%{spell}'."
        );
        break;
    case 2:
        head = _("Shrine of the 2nd Circle");
        body = _(
            "You come across an ornate shrine attended by a group of rotund friars.\nIn exchange for your protection, they agree to teach you a spell - '%{spell}'."
        );
        break;
    case 3:
        head = _("Shrine of the 3rd Circle");
        body = _(
            "You come across a lavish shrine attended by a group of high priests.\nIn exchange for your protection, they agree to teach you a sophisticated spell - '%{spell}'."
        );
        break;
    default:
        return;
    }

    StringReplace(body, "%{spell}", spell.GetName());

    // check spell book
    if (!hero.HaveSpellBook())
    {
        if (!Settings::Get().ExtHeroBuySpellBookFromShrine() || !hero.BuySpellBook(nullptr, spell_level))
        {
            PlaySoundFailure;
            body += _("\nUnfortunately, you have no Magic Book to record the spell with.");
            Message(head, body, Font::BIG, Dialog::OK);
        }
    }

    if (hero.HaveSpellBook())
    {
        // check valid level spell and wisdom skill
        if (3 == spell_level && Skill::Level::NONE == hero.GetLevelSkill(Skill::SkillT::WISDOM))
        {
            PlaySoundFailure;
            body += _(
                "\nUnfortunately, you do not have the wisdom to understand the spell, and you are unable to learn it.");
            Message(head, body, Font::BIG, Dialog::OK);
        }
        else
            // already know (skip bag artifacts)
            if (hero.HaveSpell(spell(), true))
            {
                PlaySoundFailure;
                body += _(
                    "\nUnfortunately, you already have knowledge of this spell, so there is nothing more for them to teach you."
                );
                Message(head, body, Font::BIG, Dialog::OK);
            }
            else
            {
                PlaySoundSuccess;
                hero.AppendSpellToBook(spell());
                Dialog::SpellInfo(head, body, spell());
            }
    }

    hero.SetVisited(dst_index, Visit::GLOBAL);
}

void ActionToWitchsHut(Heroes& hero, uint32_t obj, s32 dst_index)
{
    const Skill::Secondary& skill = world.GetTiles(dst_index).QuantitySkill();

    AGG::PlayMusic(MUS::SKILL, false);

    if (skill.isValid())
    {
        string msg = _(
            "You approach the hut and observe a witch inside studying an ancient tome on %{skill}.\n \n");
        const string& skill_name = Skill::Secondary::String(skill.Skill());
        StringReplace(msg, "%{skill}", skill_name);

        // check full
        if (hero.HasMaxSecondarySkill())
        {
            msg.append(
                _(
                    "As you approach, she turns and focuses her one glass eye on you.\n\"You already know everything you deserve to learn!\" the witch screeches. \"NOW GET OUT OF MY HOUSE!\""
                ));
            Message(MP2::StringObject(obj), msg, Font::BIG, Dialog::OK);
        }
        else
            // check present skill
            if (hero.HasSecondarySkill(skill.Skill()))
            {
                msg.append(
                    _(
                        "As you approach, she turns and speaks.\n\"You already know that which I would teach you. I can help you no further.\""
                    ));
                Message(MP2::StringObject(obj), msg, Font::BIG, Dialog::OK);
            }
            else
            {
                hero.LearnSkill(skill);

                msg.append(
                    _(
                        "An ancient and immortal witch living in a hut with bird's legs for stilts teaches you %{skill} for her own inscrutable purposes."
                    ));
                StringReplace(msg, "%{skill}", skill_name);
                Dialog::SecondarySkillInfo(MP2::StringObject(obj), msg, skill);
            }
    }

    hero.SetVisited(dst_index, Visit::GLOBAL);
}

void ActionToGoodLuckObject(Heroes& hero, uint32_t obj, s32 dst_index)
{
    bool visited = hero.isVisited(obj);
    string msg;

    switch (obj)
    {
    case MP2::OBJ_FOUNTAIN:
        msg = visited
                  ? _("You drink from the enchanted fountain, but nothing happens.")
                  : _("As you drink the sweet water, you gain luck for your next battle.");
        break;

    case MP2::OBJ_FAERIERING:
        msg = visited
                  ? _("You enter the faerie ring, but nothing happens.")
                  : _("Upon entering the mystical faerie ring, your army gains luck for its next battle.");
        break;

    case MP2::OBJ_IDOL:
        msg = visited
                  ? _(
                      "You've found an ancient and weathered stone idol.\nIt is supposed to grant luck to visitors, but since the stars are already smiling upon you, it does nothing."
                  )
                  : _(
                      "You've found an ancient and weathered stone idol.\nKissing it is supposed to be lucky, so you do. The stone is very cold to the touch."
                  );
        break;

    case MP2::OBJ_MERMAID:
        msg = visited
                  ? _("The mermaids silently entice you to return later and be blessed again.")
                  : _(
                      "The magical, soothing beauty of the Mermaids reaches you and your crew.\nJust for a moment, you forget your worries and bask in the beauty of the moment.\nThe mermaids charms bless you with increased luck for your next combat."
                  );
        break;

    default:
        break;
    }

    // check already visited
    if (visited)
    {
        PlaySoundVisited;
        Message(MP2::StringObject(obj), msg, Font::BIG, Dialog::OK);
    }
    else
    {
        // modify luck
        hero.SetVisited(dst_index);
        AGG::PlaySound(M82::GOODLUCK);
        DialogLuck(MP2::StringObject(obj), msg, true, 1);
    }
}

void ActionToPoorLuckObject(Heroes& hero, uint32_t obj, s32 dst_index)
{
    Maps::Tiles& tile = world.GetTiles(dst_index);
    const Spell& spell = tile.QuantitySpell();
    string ask, msg;

    switch (obj)
    {
    case MP2::OBJ_PYRAMID:
        ask = _(
            "You come upon the pyramid of a great and ancient king.\nYou are tempted to search it for treasure, but all the old stories warn of fearful curses and undead guardians.\nWill you search?"
        );
        msg = _(
            "You come upon the pyramid of a great and ancient king.\nRoutine exploration reveals that the pyramid is completely empty."
        );
        break;

    default:
        break;
    }

    if (Dialog::YES == Dialog::Message("", ask, Font::BIG, Dialog::YES | Dialog::NO))
    {
        if (spell.isValid())
        {
            PlaySoundWarning;

            // battle
            Army army(tile);

            Battle::Result res = BattleHeroWithMonster(hero, army, dst_index);
            if (res.AttackerWins())
            {
                PlaySoundSuccess;

                hero.IncreaseExperience(res.GetExperienceAttacker());
                bool valid = false;

                // check magick book
                if (!hero.HaveSpellBook())
                    msg = _("Unfortunately, you have no Magic Book to record the spell with.");
                else
                    // check skill level for wisdom
                    if (Skill::Level::EXPERT > hero.GetLevelSkill(Skill::SkillT::WISDOM))
                        msg = _(
                            "Unfortunately, you do not have the wisdom to understand the spell, and you are unable to learn it."
                        );
                    else
                    {
                        valid = true;
                        msg = _(
                            "Upon defeating the monsters, you decipher an ancient glyph on the wall, telling the secret of the spell."
                        );
                    }

                if (valid)
                {
                    Dialog::SpellInfo(spell.GetName(), msg, spell, true);
                    hero.AppendSpellToBook(spell);
                }
                else
                    Message(MP2::StringObject(obj), msg, Font::BIG, Dialog::OK);

                tile.QuantityReset();
                hero.SetVisited(dst_index, Visit::GLOBAL);
            }
            else
            {
                BattleLose(hero, res, true);
            }
        }
        else
        {
            // modify luck
            AGG::PlaySound(M82::BADLUCK);
            DialogLuck(MP2::StringObject(obj), msg, false, 2);

            hero.SetVisited(dst_index, Visit::LOCAL);
            hero.SetVisited(dst_index, Visit::GLOBAL);
        }
    }
}

void ActionToSign(Heroes& hero, uint32_t obj, s32 dst_index)
{
    PlaySoundWarning;
    Maps::Tiles& tile = world.GetTiles(dst_index);
    auto* sign = static_cast<MapSign *>(world.GetMapObject(tile.GetObjectUID(obj)));
    Message(_("Sign"), sign ? sign->message : "", Font::BIG, Dialog::OK);
}

void ActionToMagicWell(Heroes& hero, uint32_t obj, s32 dst_index)
{
    const uint32_t max = hero.GetMaxSpellPoints();

    if (hero.GetSpellPoints() == max)
    {
        PlaySoundFailure;
        Message(StringObject(MP2::OBJ_MAGICWELL),
                _("A drink at the well is supposed to restore your spell points, but you are already at maximum."),
                Font::BIG, Dialog::OK);
    }
    else
        // check already visited
        if (hero.isVisited(MP2::OBJ_MAGICWELL))
        {
            PlaySoundVisited;
            Message(StringObject(MP2::OBJ_MAGICWELL),
                    _("A second drink at the well in one day will not help you."), Font::BIG, Dialog::OK);
        }
        else
        {
            PlaySoundSuccess;
            hero.SetVisited(dst_index);
            hero.SetSpellPoints(max);
            Message(StringObject(MP2::OBJ_MAGICWELL),
                    _("A drink from the well has restored your spell points to maximum."), Font::BIG, Dialog::OK);
        }
}

void ActionToTradingPost(Heroes& hero, uint32_t obj)
{
    PlaySoundSuccess;
    Dialog::Marketplace(true);
}

void ActionToPrimarySkillObject(Heroes& hero, uint32_t obj, s32 dst_index)
{
    const Maps::Tiles& tile = world.GetTiles(dst_index);

    string msg;
    int skill = Skill::Primary::ATTACK;
    bool visited = hero.isVisited(tile);

    switch (obj)
    {
    case MP2::OBJ_FORT:
        skill = Skill::Primary::DEFENSE;
        msg = visited
                  ? _(
                      "\"I'm sorry sir,\" The leader of the soldiers says, \"but you already know everything we have to teach.\""
                  )
                  : _("The soldiers living in the fort teach you a few new defensive tricks.");
        break;

    case MP2::OBJ_MERCENARYCAMP:
        skill = Skill::Primary::ATTACK;
        msg = visited
                  ? _(
                      "You've come upon a mercenary camp practicing their tactics. \"You're too advanced for us,\" the mercenary captain says. \"We can teach nothing more.\""
                  )
                  : _(
                      "You've come upon a mercenary camp practicing their tactics. The mercenaries welcome you and your troops and invite you to train with them."
                  );
        break;

    case MP2::OBJ_DOCTORHUT:
        skill = Skill::Primary::KNOWLEDGE;
        msg = visited
                  ? _("\"Go 'way!\", the witch doctor barks, \"you know all I know.\"")
                  : _(
                      "An Orcish witch doctor living in the hut deepens your knowledge of magic by showing you how to cast stones, read portents, and decipher the intricacies of chicken entrails."
                  );
        break;

    case MP2::OBJ_STANDINGSTONES:
        skill = Skill::Primary::POWER;
        msg = visited
                  ? _(
                      "You've found a group of Druids worshipping at one of their strange stone edifices. Silently, the Druids turn you away, indicating they have nothing new to teach you."
                  )
                  : _(
                      "You've found a group of Druids worshipping at one of their strange stone edifices. Silently, they teach you new ways to cast spells."
                  );
        break;

    default:
        return;
    }

    // check already visited
    if (visited)
    {
        PlaySoundVisited;
        Message(MP2::StringObject(obj), msg, Font::BIG, Dialog::OK);
    }
    else
    {
        PlaySoundSuccess;
        // increase skill
        hero.IncreasePrimarySkill(skill);
        hero.SetVisited(dst_index);
        Dialog::PrimarySkillInfo(MP2::StringObject(obj), msg, skill);

        // fix double action tile
        hero.SetVisitedWideTile(dst_index, obj);
    }
}

void ActionToPoorMoraleObject(Heroes& hero, uint32_t obj, s32 dst_index)
{
    Maps::Tiles& tile = world.GetTiles(dst_index);
    uint32_t gold = tile.QuantityGold();
    string ask, msg, win;

    switch (obj)
    {
    case MP2::OBJ_GRAVEYARD:
        ask = _("You tentatively approach the burial ground of ancient warriors. Do you want to search the graves?");
        msg = _(
            "Upon defeating the Zombies you spend several hours searching the graves and find nothing. Such a despicable act reduces your army's morale."
        );
        win = _("Upon defeating the zomies you search the graves and find something!");
        break;
    case MP2::OBJ_SHIPWRECK:
        ask = _(
            "The rotting hulk of a great pirate ship creaks eerily as it is pushed against the rocks. Do you wish to search the shipwreck?"
        );
        msg = _(
            "Upon defeating the Ghosts you spend several hours sifting through the debris and find nothing. Such a despicable act reduces your army's morale."
        );
        win = _("Upon defeating the Ghosts you sift through the debris and find something!");
        break;
    case MP2::OBJ_DERELICTSHIP:
        ask = _(
            "The rotting hulk of a great pirate ship creaks eerily as it is pushed against the rocks. Do you wish to search the ship?"
        );
        msg = _(
            "Upon defeating the Skeletons you spend several hours sifting through the debris and find nothing. Such a despicable act reduces your army's morale."
        );
        win = _("Upon defeating the Skeletons you sift through the debris and find something!");
        break;
    default:
        break;
    }

    if (Dialog::YES == Dialog::Message("", ask, Font::BIG, Dialog::YES | Dialog::NO))
    {
        bool complete = false;

        if (gold)
        {
            PlaySoundWarning;

            Army army(tile);

            Battle::Result res = BattleHeroWithMonster(hero, army, dst_index);
            if (res.AttackerWins())
            {
                hero.IncreaseExperience(res.GetExperienceAttacker());
                complete = true;
                const Artifact& art = tile.QuantityArtifact();

                PlaySoundSuccess;

                if (art.isValid())
                {
                    if (hero.IsFullBagArtifacts())
                    {
                        gold = GoldInsteadArtifact(obj);
                        DialogWithGold(MP2::StringObject(obj), win, gold);
                    }
                    else
                    {
                        DialogWithArtifactAndGold(MP2::StringObject(obj), win, art, gold);
                        hero.PickupArtifact(art);
                    }
                }
                else
                    DialogWithGold(MP2::StringObject(obj), win, gold);

                hero.GetKingdom().AddFundsResource(Funds(Resource::GOLD, gold));
            }
            else
            {
                BattleLose(hero, res, true);
            }
        }

        if (complete)
            tile.QuantityReset();
        else if (0 == gold && !hero.isVisited(obj))
        {
            // modify morale
            hero.SetVisited(dst_index);
            hero.SetVisited(dst_index, Visit::GLOBAL);
            AGG::PlaySound(M82::BADMRLE);
            DialogMorale(MP2::StringObject(obj), msg, false, 1);
        }
    }
}

void ActionToGoodMoraleObject(Heroes& hero, uint32_t obj, s32 dst_index)
{
    string msg;
    uint32_t move = 0;
    bool visited = hero.isVisited(obj);

    switch (obj)
    {
    case MP2::OBJ_BUOY:
        msg = visited
                  ? _("Your men spot a navigational buoy, confirming that you are on course.")
                  : _(
                      "Your men spot a navigational buoy, confirming that you are on course and increasing their morale."
                  );
        break;

    case MP2::OBJ_OASIS:
        msg = visited
                  ? _(
                      "The drink at the oasis is refreshing, but offers no further benefit. The oasis might help again if you fought a battle first."
                  )
                  : _(
                      "A drink at the oasis fills your troops with strength and lifts their spirits.  You can travel a bit further today."
                  );
        move = 800; // + 8TP, from FAQ
        break;

    case MP2::OBJ_WATERINGHOLE:
        msg = visited
                  ? _(
                      "The drink at the watering hole is refreshing, but offers no further benefit. The watering hole might help again if you fought a battle first."
                  )
                  : _(
                      "A drink at the watering hole fills your troops with strength and lifts their spirits. You can travel a bit further today."
                  );
        move = 400; // + 4TP, from FAQ
        break;

    case MP2::OBJ_TEMPLE:
        msg = visited
                  ? _("It doesn't help to pray twice before a battle. Come back after you've fought.")
                  : _("A visit and a prayer at the temple raises the morale of your troops.");
        break;

    default:
        return;
    }

    // check already visited
    if (visited)
    {
        PlaySoundVisited;
        Message(MP2::StringObject(obj), msg, Font::BIG, Dialog::OK);
    }
    else
    {
        // modify morale
        hero.SetVisited(dst_index);
        AGG::PlaySound(M82::GOODMRLE);
        DialogMorale(MP2::StringObject(obj), msg, true, obj == MP2::OBJ_TEMPLE ? 2 : 1);
        hero.IncreaseMovePoints(move);

        // fix double action tile
        hero.SetVisitedWideTile(dst_index, obj);
    }
}


void ActionToExperienceObject(Heroes& hero, uint32_t obj, s32 dst_index)
{
    const Maps::Tiles& tile = world.GetTiles(dst_index);

    bool visited = hero.isVisited(tile);
    string msg;

    uint32_t exp = 0;

    switch (obj)
    {
    case MP2::OBJ_GAZEBO:
        msg = visited
                  ? _(
                      "An old Knight appears on the steps of the gazebo. \"I am sorry, my liege, I have taught you all I can.\""
                  )
                  : _(
                      "An old Knight appears on the steps of the gazebo. \"My liege, I will teach you all that I know to aid you in your travels.\""
                  );
        exp = 1000;
        break;

    default:
        return;
    }

    if (!Settings::Get().MusicMIDI())
        AGG::PlayMusic(MUS::EXPERIENCE, false);

    // check already visited
    if (visited)
    {
        if (Settings::Get().MusicMIDI())
        {
            PlaySoundVisited;
        }
        Message(MP2::StringObject(obj), msg, Font::BIG, Dialog::OK);
    }
    else
    {
        if (Settings::Get().MusicMIDI())
        {
            PlaySoundWarning;
        }
        DialogWithExp(MP2::StringObject(obj), msg, exp);

        // visit
        hero.SetVisited(dst_index);
        hero.IncreaseExperience(exp);
    }
}

void ActionToShipwreckSurvivor(Heroes& hero, uint32_t obj, s32 dst_index)
{
    Maps::Tiles& tile = world.GetTiles(dst_index);

    PlaySoundSuccess;

    if (hero.IsFullBagArtifacts())
    {
        const uint32_t gold = GoldInsteadArtifact(obj);
        DialogWithGold(MP2::StringObject(obj),
                       _(
                           "You've pulled a shipwreck survivor from certain death in an unforgiving ocean. Grateful, he says, \"I would give you an artifact as a reward, but you're all full.\""
                       ),
                       gold, Dialog::OK);
        hero.GetKingdom().AddFundsResource(Funds(Resource::GOLD, gold));
    }
    else
    {
        const Artifact& art = tile.QuantityArtifact();
        string str = _(
            "You've pulled a shipwreck survivor from certain death in an unforgiving ocean. Grateful, he rewards you for your act of kindness by giving you the %{art}."
        );
        StringReplace(str, "%{art}", art.GetName());
        Dialog::ArtifactInfo("", str, art);
        hero.PickupArtifact(art);
    }

    AnimationRemoveObject(tile);

    tile.RemoveObjectSprite();
    tile.QuantityReset();
}

void ActionToArtifact(Heroes& hero, uint32_t obj, s32 dst_index)
{
    Maps::Tiles& tile = world.GetTiles(dst_index);
    auto* map_artifact = static_cast<MapArtifact *>(world.GetMapObject(tile.GetObjectUID(obj)));

    if (hero.IsFullBagArtifacts())
        Message("", _("You have no room to carry another artifact!"), Font::BIG, Dialog::OK);
    else
    {
        uint32_t cond = tile.QuantityVariant();
        Artifact art = tile.QuantityArtifact();

        if (map_artifact)
        {
            cond = map_artifact->condition;
            art = map_artifact->artifact;
        }

        bool result = false;
        string msg;

        // 1,2,3 - gold, gold + res
        if (0 < cond && cond < 4)
        {
            Funds payment = map_artifact ? map_artifact->QuantityFunds() : tile.QuantityFunds();

            if (1 == cond)
            {
                msg = _("A leprechaun offers you the %{art} for the small price of %{gold} Gold.");
                StringReplace(msg, "%{gold}", payment.Get(Resource::GOLD));
            }
            else
            {
                msg = _("A leprechaun offers you the %{art} for the small price of %{gold} Gold and %{count} %{res}.");

                StringReplace(msg, "%{gold}", payment.Get(Resource::GOLD));
                ResourceCount rc = map_artifact ? map_artifact->QuantityResourceCount() : tile.QuantityResourceCount();
                StringReplace(msg, "%{count}", rc.second);
                StringReplace(msg, "%{res}", Resource::String(rc.first));
            }
            StringReplace(msg, "%{art}", art.GetName());
            msg.append("\n");
            msg.append(_("Do you wish to buy this artifact?"));

            PlaySoundWarning;
            if (Dialog::YES == Dialog::ArtifactInfo("", msg, art, Dialog::YES | Dialog::NO))
            {
                if (hero.GetKingdom().AllowPayment(payment))
                {
                    result = true;
                    hero.GetKingdom().OddFundsResource(payment);
                }
                else
                {
                    PlaySoundFailure;
                    Message("",
                            _(
                                "You try to pay the leprechaun, but realize that you can't afford it. The leprechaun stamps his foot and ignores you."
                            ),
                            Font::BIG, Dialog::OK);
                }
            }
            else
                Message("",
                        _(
                            "Insulted by your refusal of his generous offer, the leprechaun stamps his foot and ignores you."
                        ),
                        Font::BIG, Dialog::OK);
        }
        else
            // 4,5 - need have skill wisard or leadership,
            if (3 < cond && cond < 6)
            {
                const Skill::Secondary& skill = tile.QuantitySkill();

                if (hero.HasSecondarySkill(skill.Skill()))
                {
                    PlaySoundSuccess;
                    msg = _("You've found the artifact: ");
                    msg.append(art.GetName());
                    ArtifactInfo("", msg, art, Dialog::OK);
                    result = true;
                }
                else
                {
                    PlaySoundFailure;

                    if (skill.Skill() == Skill::SkillT::WISDOM)
                        msg = _(
                            "You've found the humble dwelling of a withered hermit. The hermit tells you that he is willing to give the %{art} to the first wise person he meets."
                        );
                    else if (skill.Skill() == Skill::SkillT::LEADERSHIP)
                        msg = _(
                            "You've come across the spartan quarters of a retired soldier. The soldier tells you that he is willing to pass on the %{art} to the first true leader he meets."
                        );
                    else
                        msg = "FIXME: (unknown condition): %{art}";

                    StringReplace(msg, "%{art}", art.GetName());
                    Message("", msg, Font::BIG, Dialog::OK);
                }
            }
            else
                // 6 - 50 rogues, 7 - 1 gin, 8,9,10,11,12,13 - 1 monster level4
                if (5 < cond && cond < 14)
                {
                    bool battle = true;
                    Army army(tile);
                    Troop* troop = army.m_troops.GetFirstValid();

                    PlaySoundWarning;

                    if (troop)
                    {
                        if (Monster::ROGUE == troop->_monster.GetID())
                            Message("",
                                    _(
                                        "You come upon an ancient artifact. As you reach for it, a pack of Rogues leap out of the brush to guard their stolen loot."
                                    ),
                                    Font::BIG, Dialog::OK);
                        else
                        {
                            msg = _(
                                "Through a clearing you observe an ancient artifact. Unfortunately, it's guarded by a nearby %{monster}. Do you want to fight the %{monster} for the artifact?"
                            );
                            StringReplace(msg, "%{monster}", troop->GetName());
                            battle = Dialog::YES == Dialog::Message("", msg, Font::BIG, Dialog::YES | Dialog::NO);
                        }
                    }

                    if (battle)
                    {
                        // new battle
                        Battle::Result res = BattleHeroWithMonster(hero, army, dst_index);
                        if (res.AttackerWins())
                        {
                            hero.IncreaseExperience(res.GetExperienceAttacker());
                            result = true;
                            PlaySoundSuccess;
                            msg = _("Victorious, you take your prize, the %{art}.");
                            StringReplace(msg, "%{art}", art.GetName());
                            Dialog::ArtifactInfo("", msg, art());
                        }
                        else
                        {
                            BattleLose(hero, res, true);
                        }
                    }
                    else
                    {
                        PlaySoundFailure;
                        Message("",
                                _(
                                    "Discretion is the better part of valor, and you decide to avoid this fight for today."
                                ),
                                Font::BIG, Dialog::OK);
                    }
                }
                else
                {
                    PlaySoundSuccess;

                    if (!Artifact::GetScenario(art).empty())
                        msg = Artifact::GetScenario(art);
                    else
                    {
                        msg = _("You've found the artifact: ");
                        msg.append("\n");
                        msg.append(art.GetName());
                    }

                    Dialog::ArtifactInfo("", msg, art);
                    result = true;
                }

        if (result && hero.PickupArtifact(art))
        {
            Game::PlayPickupSound();
            AnimationRemoveObject(tile);

            tile.RemoveObjectSprite();
            tile.QuantityReset();
        }
    }
}

void ActionToTreasureChest(Heroes& hero, uint32_t obj, s32 dst_index)
{
    Maps::Tiles& tile = world.GetTiles(dst_index);
    const string& hdr = MP2::StringObject(obj);

    string msg;
    uint32_t gold = tile.QuantityGold();

    Game::PlayPickupSound();
    AnimationRemoveObject(tile);

    // dialog
    if (tile.isWater())
    {
        if (gold)
        {
            const Artifact& art = tile.QuantityArtifact();

            if (art.isValid())
            {
                if (hero.IsFullBagArtifacts())
                {
                    gold = GoldInsteadArtifact(obj);
                    msg = _(
                        "After spending hours trying to fish the chest out of the sea, you open it and find %{gold} gold pieces."
                    );
                    StringReplace(msg, "%{gold}", gold);
                    DialogWithGold(hdr, msg, gold);
                }
                else
                {
                    msg = _(
                        "After spending hours trying to fish the chest out of the sea, you open it and find %{gold} gold and the %{art}."
                    );
                    StringReplace(msg, "%{gold}", gold);
                    StringReplace(msg, "%{art}", art.GetName());
                    DialogWithArtifactAndGold(hdr, msg, art, gold);
                    hero.PickupArtifact(art);
                }
            }
            else
            {
                msg = _(
                    "After spending hours trying to fish the chest out of the sea, you open it and find %{gold} gold pieces."
                );
                StringReplace(msg, "%{gold}", gold);
                DialogWithGold(hdr, msg, gold);
            }
        }
        else
        {
            Message(hdr,
                    _(
                        "After spending hours trying to fish the chest out of the sea, you open it, only to find it empty."
                    ),
                    Font::BIG, Dialog::OK);
        }
    }
    else
    {
        const Artifact& art = tile.QuantityArtifact();

        if (gold)
        {
            const uint32_t expr = gold > 500 ? gold - 500 : 500;
            msg = _(
                "After scouring the area, you fall upon a hidden treasure cache. You may take the gold or distribute the gold to the peasants for experience. Do you wish to keep the gold?"
            );

            if (!Dialog::SelectGoldOrExp(hdr, msg, gold, expr, hero))
            {
                gold = 0;
                hero.IncreaseExperience(expr);
            }
        }
        else if (art.isValid())
        {
            if (hero.IsFullBagArtifacts())
            {
                gold = GoldInsteadArtifact(obj);
                msg = _("After scouring the area, you fall upon a hidden chest, containing the %{gold} gold pieces.");
                StringReplace(msg, "%{gold}", gold);
                DialogWithGold(hdr, msg, gold);
            }
            else
            {
                msg = _("After scouring the area, you fall upon a hidden chest, containing the ancient artifact %{art}."
                );
                StringReplace(msg, "%{art}", art.GetName());
                Dialog::ArtifactInfo("", msg, art);
                hero.PickupArtifact(art);
            }
        }
    }

    if (gold)
        hero.GetKingdom().AddFundsResource(Funds(Resource::GOLD, gold));

    tile.RemoveObjectSprite();
    tile.QuantityReset();
}

void ActionToAncientLamp(Heroes& hero, uint32_t obj, s32 dst_index)
{
    Maps::Tiles& tile = world.GetTiles(dst_index);
    const Troop& troop = tile.QuantityTroop();

    PlaySoundSuccess;
    if (troop.isValid() &&
        Dialog::YES == Dialog::Message(MP2::StringObject(obj),
                                       _(
                                           "You stumble upon a dented and tarnished lamp lodged deep in the earth. Do you wish to rub the lamp?"
                                       ),
                                       Font::BIG, Dialog::YES | Dialog::NO))
        RecruitMonsterFromTile(hero, tile, MP2::StringObject(obj), troop, true);
}

void ActionToTeleports(Heroes& hero, s32 index_from)
{
    s32 index_to = world.NextTeleport(index_from);
    hero.ApplyPenaltyMovement();

    if (index_from == index_to)
    {
        AGG::PlaySound(M82::RSBRYFZL);
        return;
    }

    const Heroes* other_hero = world.GetTiles(index_to).GetHeroes();
    if (other_hero)
    {
        ActionToHeroes(hero, MP2::OBJ_STONELIGHTS, index_to);

        // lose battle
        if (hero.isFreeman())
            return;
        if (!other_hero->isFreeman())
        {
        }
    }

    AGG::PlaySound(M82::KILLFADE);
    hero.GetPath().Hide();
    hero.FadeOut();

    Cursor::Get().Hide();

    hero.Move2Dest(index_to, true);

    Interface::Basic& I = Interface::Basic::Get();
    I.GetGameArea().SetCenter(hero.GetCenter());
    I.RedrawFocus();
    I.Redraw();

    AGG::PlaySound(M82::KILLFADE);
    hero.GetPath().Hide();
    hero.FadeIn();

    hero.GetPath().Reset();
    hero.ActionNewPosition();
}

void ActionToWhirlpools(Heroes& hero, uint32_t obj, s32 index_from)
{
    const s32 index_to = world.NextWhirlpool(index_from);
    hero.ApplyPenaltyMovement();

    if (index_from == index_to)
    {
        AGG::PlaySound(M82::RSBRYFZL);
        return;
    }

    AGG::PlaySound(M82::KILLFADE);
    hero.GetPath().Hide();
    hero.FadeOut();

    Cursor::Get().Hide();

    hero.Move2Dest(index_to, true);

    Interface::Basic& I = Interface::Basic::Get();
    I.GetGameArea().SetCenter(hero.GetCenter());
    I.RedrawFocus();
    I.Redraw();

    AGG::PlaySound(M82::KILLFADE);
    hero.GetPath().Hide();
    hero.FadeIn();

    Troop* troop = hero.GetArmy().m_troops.GetWeakestTroop();

    if (troop && Rand::Get(1) && 1 < troop->GetCount())
    {
        PlaySoundWarning;
        Message(_("A whirlpool engulfs your ship."), _("Some of your army has fallen overboard."), Font::BIG,
                Dialog::OK);
        troop->SetCount(
            Monster::GetCountFromHitPoints(troop->_monster.GetID(),
                                           troop->GetHitPointsTroop() - troop->GetHitPointsTroop() *
                                           Game::GetWhirlpoolPercent() /
                                           100));
    }

    hero.GetPath().Reset();
    hero.ActionNewPosition();
}

void ActionToAbandoneMine(Heroes& hero, uint32_t obj, s32 dst_index)
{
    PlaySoundWarning;

    if (Dialog::YES == Dialog::Message("",
                                       _(
                                           "You come upon an abandoned gold mine. The mine appears to be haunted. Do you wish to enter?"
                                       ),
                                       Font::BIG, Dialog::YES | Dialog::NO))
    {
        ActionToCaptureObject(hero, obj, dst_index);
    }
}

/* capture color object */
void ActionToCaptureObject(Heroes& hero, uint32_t obj, s32 dst_index)
{
    Maps::Tiles& tile = world.GetTiles(dst_index);
    string header;
    string body;
    uint32_t resource = Resource::UNKNOWN;

    switch (obj)
    {
    case MP2::OBJ_ALCHEMYLAB:
        resource = Resource::MERCURY;
        header = MP2::StringObject(obj);
        body = _(
            "You have taken control of the local Alchemist shop. It will provide you with %{count} unit of Mercury per day."
        );
        break;

    case MP2::OBJ_SAWMILL:
        resource = Resource::WOOD;
        header = MP2::StringObject(obj);
        body = _("You gain control of a sawmill. It will provide you with %{count} units of wood per day.");
        break;

    case MP2::OBJ_MINES:
        {
            resource = tile.QuantityResourceCount().first;
            header = Maps::GetMinesName(resource);

            switch (resource)
            {
            case Resource::ORE:
                body = _(
                    "You gain control of an ore mine. It will provide you with %{count} units of ore per day.");
                break;
            case Resource::SULFUR:
                body = _(
                    "You gain control of a sulfur mine. It will provide you with %{count} unit of sulfur per day.");
                break;
            case Resource::CRYSTAL:
                body = _(
                    "You gain control of a crystal mine. It will provide you with %{count} unit of crystal per day.");
                break;
            case Resource::GEMS:
                body = _("You gain control of a gem mine. It will provide you with %{count} unit of gems per day.");
                break;
            case Resource::GOLD:
                body = _("You gain control of a gold mine. It will provide you with %{count} gold per day.");
                break;
            default:
                break;
            }
        }
        break;

    case MP2::OBJ_ABANDONEDMINE:
        body = _("You beat the Ghosts and are able to restore the mine to production.");
        break;

    case MP2::OBJ_LIGHTHOUSE:
        header = MP2::StringObject(obj);
        body = _(
            "The lighthouse is now under your control, and all of your ships will now move further each turn.");
        break;

    default:
        body = _("You gain control of a %{name}.");
        StringReplace(body, "%{name}", MP2::StringObject(obj));
        break;
    }

    // capture object
    if (!hero.isFriends(tile.QuantityColor()))
    {
        bool capture = true;

        // check guardians
        if (tile.CaptureObjectIsProtection())
        {
            Army army(tile);
            const Monster& mons = tile.QuantityMonster();

            Battle::Result result = BattleHeroWithMonster(hero, army, dst_index);

            if (result.AttackerWins())
            {
                hero.IncreaseExperience(result.GetExperienceAttacker());
            }
            else
            {
                capture = false;
                BattleLose(hero, result, true);
                if (Settings::Get().ExtWorldSaveMonsterBattle())
                    tile.MonsterSetCount(army.m_troops.GetCountMonsters(mons));
            }
        }

        if (capture)
        {
            if (resource == Resource::UNKNOWN)
                Message(header, body, Font::BIG, Dialog::OK);
            else
                DialogCaptureResourceObject(header, body, resource);

            // update abandone mine
            if (obj == MP2::OBJ_ABANDONEDMINE)
            {
                Maps::Tiles::UpdateAbandoneMineSprite(tile);
                hero.SetMapsObject(MP2::OBJ_MINES);
            }

            // reset spell info
            Maps::TilesAddon* addon = tile.FindObject(MP2::OBJ_MINES);
            if (addon) addon->tmp = 0;

            tile.QuantitySetColor(hero.GetColor());

            if (MP2::OBJ_LIGHTHOUSE == obj)
                Maps::ClearFog(dst_index, GetViewDistance(Game::VIEW_LIGHT_HOUSE), hero.GetColor());
        }
    }
    else
        // set guardians
        if (Settings::Get().ExtWorldAllowSetGuardian())
        {
            CapturedObject& co = world.GetCapturedObject(dst_index);
            Troop& troop1 = co.GetTroop();
            Troop troop2 = troop1;

            // check set with spell ?
            Maps::TilesAddon* addon = tile.FindObject(MP2::OBJ_MINES);
            bool readonly = addon ? addon->tmp : false;

            if (Dialog::SetGuardian(hero, troop2, co, readonly))
                troop1.Set(troop2(), troop2.GetCount());
        }
}

void ActionToDwellingJoinMonster(Heroes& hero, uint32_t obj, s32 dst_index)
{
    Maps::Tiles& tile = world.GetTiles(dst_index);
    const Troop& troop = tile.QuantityTroop();

    if (troop.isValid())
    {
        hero.MovePointsScaleFixed();

        string message = _(
            "A group of %{monster} with a desire for greater glory wish to join you. Do you accept?");
        StringReplace(message, "%{monster}", troop._monster.GetMultiName());

        if (!Settings::Get().MusicMIDI() && obj == MP2::OBJ_WATCHTOWER)
            AGG::PlayMusic(MUS::WATCHTOWER, false);
        else
            AGG::PlaySound(M82::EXPERNCE);

        if (Dialog::YES == Dialog::Message(MP2::StringObject(obj), message, Font::BIG, Dialog::YES | Dialog::NO))
        {
            if (!hero.GetArmy().m_troops.CanJoinTroop(troop._monster))
                Message(troop.GetName(), _("You are unable to recruit at this time, your ranks are full."),
                        Font::BIG, Dialog::OK);
            else
            {
                tile.MonsterSetCount(0);
                hero.GetArmy().m_troops.JoinTroop(troop);
                hero.MovePointsScaleFixed();

                if (Settings::Get().ExtHeroRecalculateMovement())
                    hero.RecalculateMovePoints();

                Interface::Basic::Get().GetStatusWindow().SetRedraw();
            }
        }
    }
    else
    {
        Message("", _("As you approach the dwelling, you notice that there is no one here."), Font::BIG,
                Dialog::OK);
    }

    hero.SetVisited(dst_index, Visit::GLOBAL);
}

void ActionToDwellingRecruitMonster(Heroes& hero, uint32_t obj, s32 dst_index)
{
    Maps::Tiles& tile = world.GetTiles(dst_index);

    string msg_full, msg_void;

    switch (obj)
    {
    case MP2::OBJ_RUINS:
        msg_void = _(
            "You search the ruins, but the Medusas that used to live here are gone. Perhaps there will be more next week."
        );
        msg_full = _(
            "You've found some Medusas living in the ruins. They are willing to join your army for a price. Do you want to recruit Medusas?"
        );
        AGG::PlayMusic(MUS::DEATH, false);
        break;

    case MP2::OBJ_TREECITY:
        msg_void = _(
            "You've found a Sprite Tree City. Unfortunately, none of the Sprites living there wish to join an army. Maybe next week."
        );
        msg_full = _(
            "Some of the Sprites living in the tree city are willing to join your army for a price. Do you want to recruit Sprites?"
        );
        AGG::PlayMusic(MUS::TREEHOUSE, false);
        break;

    case MP2::OBJ_WAGONCAMP:
        msg_void = _("A colorful Rogues' wagon stands empty here. Perhaps more Rogues will be here later.");
        msg_full = _(
            "Distant sounds of music and laughter draw you to a colorful wagon housing Rogues. Do you wish to have any Rogues join your army?"
        );
        AGG::PlayMusic(MUS::ARABIAN, false);
        break;

    case MP2::OBJ_DESERTTENT:
        msg_void = _(
            "A group of tattered tents, billowing in the sandy wind, beckons you. The tents are unoccupied. Perhaps more Nomads will be here later."
        );
        msg_full = _(
            "A group of tattered tents, billowing in the sandy wind, beckons you. Do you wish to have any Nomads join you during your travels?"
        );
        AGG::PlayMusic(MUS::NOMADTENTS, false);
        break;

    case MP2::OBJ_EARTHALTAR:
        msg_void = _("The pit of mud bubbles for a minute and then lies still.");
        msg_full = _(
            "As you approach the bubbling pit of mud, creatures begin to climb out and position themselves around it. In unison they say: \"Mother Earth would like to offer you a few of her troops. Do you want to recruit Earth Elementals?\""
        );
        break;

    case MP2::OBJ_AIRALTAR:
        msg_void = _("You enter the structure of white stone pillars, and find nothing.");
        msg_full = _(
            "White stone pillars support a roof that rises up to the sky. As you enter the structure, the dead air of the outside gives way to a whirling gust that almost pushes you back out. The air current materializes into a barely visible form. The creature asks, in what can only be described as a loud whisper: \"Why have you come? Are you here to call upon the forces of the air?\""
        );
        break;

    case MP2::OBJ_FIREALTAR:
        msg_void = _("No Fire Elementals approach you from the lava pool.");
        msg_full = _(
            "Beneath a structure that serves to hold in heat, Fire Elementals move about in a fiery pool of molten lava. A group of them approach you and offer their services. Would you like to recruit Fire Elementals?"
        );
        break;

    case MP2::OBJ_WATERALTAR:
        msg_void = _("A face forms in the water for a moment, and then is gone.");
        msg_full = _(
            "Crystalline structures cast shadows over a small reflective pool of water. You peer into the pool, and a face that is not your own peers back. It asks: \"Would you like to call upon the powers of water?\""
        );
        break;

    case MP2::OBJ_BARROWMOUNDS:
        msg_void = _("This burial site is deathly still.");
        msg_full = _(
            "Restless spirits of long dead warriors seeking their final resting place offer to join you in hopes of finding peace. Do you wish to recruit ghosts?"
        );
        break;

    default:
        return;
    }

    const Troop& troop = tile.QuantityTroop();

    if (!troop.isValid())
        Message(MP2::StringObject(obj), msg_void, Font::BIG, Dialog::OK);
    else if (Dialog::YES == Dialog::Message(MP2::StringObject(obj), msg_full, Font::BIG, Dialog::YES | Dialog::NO))
        RecruitMonsterFromTile(hero, tile, MP2::StringObject(obj), troop, false);

    hero.SetVisited(dst_index, Visit::GLOBAL);
}

void ActionToDwellingBattleMonster(Heroes& hero, uint32_t obj, s32 dst_index)
{
    Maps::Tiles& tile = world.GetTiles(dst_index);

    // yet no one captured.
    const bool& battle = Color::NONE == tile.QuantityColor();
    const Troop& troop = tile.QuantityTroop();

    string str_empty;
    string str_recr;
    string str_warn;
    string str_wins;
    string str_scss;

    switch (obj)
    {
    case MP2::OBJ_CITYDEAD:
        str_empty = _(
            "The City of the Dead is empty of life, and empty of unlife as well. Perhaps some undead will move in next week."
        );
        str_recr = _(
            "Some Liches living here are willing to join your army for a price. Do you want to recruit Liches?");
        str_warn = _(
            "You've found the ruins of an ancient city, now inhabited solely by the undead. Will you search?");
        str_wins = _(
            "Some of the surviving Liches are impressed by your victory over their fellows, and offer to join you for a price. Do you want to recruit Liches?"
        );
        break;

    case MP2::OBJ_TROLLBRIDGE:
        str_empty = _(
            "You've found one of those bridges that Trolls are so fond of living under, but there are none here. Perhaps there will be some next week."
        );
        str_recr = _(
            "Some Trolls living under a bridge are willing to join your army, but for a price. Do you want to recruit Trolls?"
        );
        str_warn = _("Trolls living under the bridge challenge you. Will you fight them?");
        str_wins = _(
            "A few Trolls remain, cowering under the bridge. They approach you and offer to join your forces as mercenaries. Do you want to buy any Trolls?"
        );
        break;

    case MP2::OBJ_DRAGONCITY:
        str_empty = _(
            "The Dragon city has no Dragons willing to join you this week. Perhaps a Dragon will become available next week."
        );
        str_recr = _(
            "The Dragon city is willing to offer some Dragons for your army for a price. Do you wish to recruit Dragons?"
        );
        str_warn = _(
            "You stand before the Dragon City, a place off-limits to mere humans. Do you wish to violate this rule and challenge the Dragons to a fight?"
        );
        str_wins = _(
            "Having defeated the Dragon champions, the city's leaders agree to supply some Dragons to your army for a price. Do you wish to recruit Dragons?"
        );
        break;

    default:
        return;
    }

    if (!battle)
    {
        if (troop.isValid())
        {
            PlaySoundSuccess;
            str_scss = str_recr;
        }
        else
        {
            PlaySoundVisited;
            Message(MP2::StringObject(obj), str_empty, Font::BIG, Dialog::OK);
        }
    }
    else
    {
        Army army(tile);

        PlaySoundWarning;
        if (Dialog::YES == Dialog::Message(MP2::StringObject(obj), str_warn, Font::BIG, Dialog::YES | Dialog::NO))
        {
            // new battle
            Battle::Result res = BattleHeroWithMonster(hero, army, dst_index);
            if (res.AttackerWins())
            {
                hero.IncreaseExperience(res.GetExperienceAttacker());
                tile.QuantitySetColor(hero.GetColor());
                tile.SetObjectPassable(true);
                PlaySoundSuccess;
                str_scss = str_wins;
            }
            else
            {
                BattleLose(hero, res, true);
            }
        }
    }

    // recruit monster
    if (!str_scss.empty())
    {
        if (troop.isValid() &&
            Dialog::YES == Dialog::Message(MP2::StringObject(obj), str_scss, Font::BIG, Dialog::YES | Dialog::NO))
            RecruitMonsterFromTile(hero, tile, MP2::StringObject(obj), troop, false);

        hero.SetVisited(dst_index, Visit::GLOBAL);
    }
}

void ActionToObservationTower(Heroes& hero, uint32_t obj, s32 dst_index)
{
    PlaySoundWarning;
    Message(MP2::StringObject(obj), _("From the observation tower, you are able to see distant lands."),
            Font::BIG, Dialog::OK);
    Maps::ClearFog(dst_index, GetViewDistance(Game::VIEW_OBSERVATION_TOWER), hero.GetColor());
}

void ActionToArtesianSpring(Heroes& hero, uint32_t obj, s32 dst_index)
{
    const uint32_t max = hero.GetMaxSpellPoints();
    const string& name = StringObject(MP2::OBJ_ARTESIANSPRING);

    if (hero.isVisited(MP2::OBJ_ARTESIANSPRING))
    {
        PlaySoundVisited;
        Message(name, _("The spring only refills once a week, and someone's already been here this week."),
                Font::BIG, Dialog::OK);
    }
    else if (hero.GetSpellPoints() == max * 2)
    {
        PlaySoundFailure;
        Message(name,
                _(
                    "A drink at the spring is supposed to give you twice your normal spell points, but you are already at that level."
                ),
                Font::BIG, Dialog::OK);
    }
    else
    {
        if (Settings::Get().MusicMIDI())
        {
            PlaySoundSuccess;
        }
        else
            AGG::PlayMusic(MUS::WATERSPRING, false);
        hero.SetSpellPoints(max * 2);
        Message(name,
                _(
                    "A drink from the spring fills your blood with magic! You have twice your normal spell points in reserve."
                ),
                Font::BIG, Dialog::OK);

        if (Settings::Get().ExtWorldArtesianSpringSeparatelyVisit())
            hero.SetVisited(dst_index, Visit::LOCAL);
        else
            // fix double action tile
            hero.SetVisitedWideTile(dst_index, obj, Visit::LOCAL);
    }
}

void ActionToXanadu(Heroes& hero, uint32_t obj, s32 dst_index)
{
    const Maps::Tiles& tile = world.GetTiles(dst_index);

    if (hero.isVisited(tile))
    {
        PlaySoundVisited;
        Message(MP2::StringObject(obj),
                _(
                    "Recognizing you, the butler refuses to admit you. \"The master,\" he says, \"will not see the same student twice.\""
                ),
                Font::BIG, Dialog::OK);
    }
    else
    {
        bool access = false;
        switch (hero.GetLevelSkill(Skill::SkillT::DIPLOMACY))
        {
        case Skill::Level::BASIC:
            if (7 < hero.GetLevel()) access = true;
            break;
        case Skill::Level::ADVANCED:
            if (5 < hero.GetLevel()) access = true;
            break;
        case Skill::Level::EXPERT:
            if (3 < hero.GetLevel()) access = true;
            break;
        default:
            if (9 < hero.GetLevel()) access = true;
            break;
        }

        if (access)
        {
            PlaySoundSuccess;
            Message(MP2::StringObject(obj),
                    _(
                        "The butler admits you to see the master of the house. He trains you in the four skills a hero should know."
                    ),
                    Font::BIG, Dialog::OK);
            hero.IncreasePrimarySkill(Skill::Primary::ATTACK);
            hero.IncreasePrimarySkill(Skill::Primary::DEFENSE);
            hero.IncreasePrimarySkill(Skill::Primary::KNOWLEDGE);
            hero.IncreasePrimarySkill(Skill::Primary::POWER);
            hero.SetVisited(dst_index);
        }
        else
        {
            PlaySoundFailure;
            Message(MP2::StringObject(obj),
                    _(
                        "The butler opens the door and looks you up and down. \"You are neither famous nor diplomatic enough to be admitted to see my master,\" he sniffs. \"Come back when you think yourself worthy.\""
                    ),
                    Font::BIG, Dialog::OK);
        }
    }
}

bool ActionToUpgradeArmy(Army& army, const Monster& mons, string& str1, string& str2)
{
    if (army.m_troops.HasMonster(mons))
    {
        army.m_troops.UpgradeMonsters(mons);
        if (!str1.empty()) str1 += ", ";
        str1 += mons.GetMultiName();
        if (!str2.empty()) str2 += ", ";
        str2 += mons.GetUpgrade().GetMultiName();
        return true;
    }
    return false;
}

void ActionToUpgradeArmyObject(Heroes& hero, uint32_t obj, s32 dst_index)
{
    string monsters;
    string monsters_upgrade;
    string msg1;
    string msg2;

    vector<Monster> mons;
    mons.reserve(3);

    hero.MovePointsScaleFixed();

    switch (obj)
    {
    case MP2::OBJ_HILLFORT:
        if (ActionToUpgradeArmy(hero.GetArmy(), Monster(Monster::DWARF), monsters, monsters_upgrade))
            mons.emplace_back(Monster::DWARF);
        if (ActionToUpgradeArmy(hero.GetArmy(), Monster(Monster::ORC), monsters, monsters_upgrade))
            mons.emplace_back(Monster::ORC);
        if (ActionToUpgradeArmy(hero.GetArmy(), Monster(Monster::OGRE), monsters, monsters_upgrade))
            mons.emplace_back(Monster::OGRE);

        msg1 = _(
            "All of the %{monsters} you have in your army have been trained by the battle masters of the fort. Your army now contains %{monsters2}."
        );
        StringReplace(msg1, "%{monsters}", monsters);
        StringReplace(msg1, "%{monsters2}", monsters_upgrade);
        msg2 = _(
            "An unusual alliance of Orcs, Ogres, and Dwarves offer to train (upgrade) any such troops brought to them. Unfortunately, you have none with you."
        );
        break;

    case MP2::OBJ_FREEMANFOUNDRY:
        if (ActionToUpgradeArmy(hero.GetArmy(), Monster(Monster::PIKEMAN), monsters, monsters_upgrade))
            mons.emplace_back(Monster::PIKEMAN);
        if (ActionToUpgradeArmy(hero.GetArmy(), Monster(Monster::SWORDSMAN), monsters, monsters_upgrade))
            mons.emplace_back(Monster::SWORDSMAN);
        if (ActionToUpgradeArmy(hero.GetArmy(), Monster(Monster::IRON_GOLEM), monsters, monsters_upgrade))
            mons.emplace_back(Monster::IRON_GOLEM);

        msg1 = _("All of your %{monsters} have been upgraded into %{monsters2}.");
        StringReplace(msg1, "%{monsters}", monsters);
        StringReplace(msg1, "%{monsters2}", monsters_upgrade);
        msg2 = _(
            "A blacksmith working at the foundry offers to convert all Pikemen and Swordsmen's weapons brought to him from iron to steel. He also says that he knows a process that will convert Iron Golems into Steel Golems.  Unfortunately, you have none of these troops in your army, so he can't help you."
        );
        break;

    default:
        break;
    }

    if (mons.empty())
    {
        PlaySoundFailure;
        Message(MP2::StringObject(obj), msg2, Font::BIG, Dialog::OK);
        return;
    }
    // composite sprite
    uint32_t ox = 0;
    const Sprite& br = AGG::GetICN(ICN::STRIP, 12);
    Surface sf(Size(br.w() * mons.size() + (mons.size() - 1) * 4, br.h()), false);

    for (vector<Monster>::const_iterator
         it = mons.begin(); it != mons.end(); ++it)
    {
        br.Blit(ox, 0, sf);
        switch (Monster(*it).GetRace())
        {
        case Race::KNGT:
            AGG::GetICN(ICN::STRIP, 4).Blit(ox + 6, 6, sf);
            break;
        case Race::BARB:
            AGG::GetICN(ICN::STRIP, 5).Blit(ox + 6, 6, sf);
            break;
        case Race::SORC:
            AGG::GetICN(ICN::STRIP, 6).Blit(ox + 6, 6, sf);
            break;
        case Race::WRLK:
            AGG::GetICN(ICN::STRIP, 7).Blit(ox + 6, 6, sf);
            break;
        case Race::WZRD:
            AGG::GetICN(ICN::STRIP, 8).Blit(ox + 6, 6, sf);
            break;
        case Race::NECR:
            AGG::GetICN(ICN::STRIP, 9).Blit(ox + 6, 6, sf);
            break;
        default:
            AGG::GetICN(ICN::STRIP, 10).Blit(ox + 6, 6, sf);
            break;
        }
        const Sprite& mon = AGG::GetICN((*it).GetUpgrade().ICNMonh(), 0);
        mon.Blit(ox + 6 + mon.x(), 6 + mon.y(), sf);
        ox += br.w() + 4;
    }
    Dialog::SpriteInfo(MP2::StringObject(obj), msg1, sf);

    if (Settings::Get().ExtHeroRecalculateMovement())
        hero.RecalculateMovePoints();
}

void ActionToMagellanMaps(Heroes& hero, uint32_t obj, s32 dst_index)
{
    const Funds payment(Resource::GOLD, 1000);
    Kingdom& kingdom = hero.GetKingdom();

    if (hero.isVisited(obj, Visit::GLOBAL))
    {
        Message(MP2::StringObject(obj), "empty", Font::BIG, Dialog::OK);
    }
    else if (kingdom.AllowPayment(payment))
    {
        PlaySoundWarning;
        if (Dialog::YES == Dialog::Message(MP2::StringObject(obj),
                                           _(
                                               "A retired captain living on this refurbished fishing platform offers to sell you maps of the sea he made in his younger days for 1,000 gold. Do you wish to buy the maps?"
                                           ),
                                           Font::BIG, Dialog::YES | Dialog::NO))
        {
            world.ActionForMagellanMaps(hero.GetColor());
            kingdom.OddFundsResource(payment);
            hero.SetVisited(dst_index, Visit::GLOBAL);
        }

        Interface::Basic& I = Interface::Basic::Get();
        I.RedrawFocus();
    }
    else
    {
        PlaySoundFailure;
        Message(MP2::StringObject(obj),
                _(
                    "The captain sighs. \"You don't have enough money, eh?  You can't expect me to give my maps away for free!\""
                ),
                Font::BIG, Dialog::OK);
    }
}

void ActionToEvent(Heroes& hero, uint32_t obj, s32 dst_index)
{
    // check event maps
    MapEvent* event_maps = world.GetMapEvent(Maps::GetPoint(dst_index));

    if (event_maps && event_maps->isAllow(hero.GetColor()))
    {
        hero.SetMove(false);

        if (event_maps->resources.GetValidItemsCount())
        {
            hero.GetKingdom().AddFundsResource(event_maps->resources);
            PlaySoundSuccess;
            Dialog::ResourceInfo("", event_maps->message, event_maps->resources);
        }
        else if (!event_maps->message.empty())
            Message("", event_maps->message, Font::BIG, Dialog::OK);

        const Artifact& art = event_maps->artifact;
        if (art.isValid())
        {
            if (hero.PickupArtifact(art))
            {
                Game::PlayPickupSound();
                string message(_("You find %{artifact}."));
                StringReplace(message, "%{artifact}", art.GetName());
                Dialog::ArtifactInfo("", message, art);
            }
        }

        event_maps->SetVisited(hero.GetColor());

        if (event_maps->cancel)
        {
            hero.SetMapsObject(MP2::OBJ_ZERO);
            world.RemoveMapObject(event_maps);
        }
    }
}

void ActionToObelisk(Heroes& hero, uint32_t obj, s32 dst_index)
{
    Kingdom& kingdom = hero.GetKingdom();
    if (!hero.isVisited(world.GetTiles(dst_index), Visit::GLOBAL))
    {
        hero.SetVisited(dst_index, Visit::GLOBAL);
        kingdom.PuzzleMaps().Update(kingdom.CountVisitedObjects(MP2::OBJ_OBELISK), world.CountObeliskOnMaps());
        AGG::PlaySound(M82::EXPERNCE);
        Message(MP2::StringObject(obj),
                _(
                    "You come upon an obelisk made from a type of stone you have never seen before. Staring at it intensely, the smooth surface suddenly changes to an inscription. The inscription is a piece of a lost ancient map. Quickly you copy down the piece and the inscription vanishes as abruptly as it appeared."
                ),
                Font::BIG, Dialog::OK);
        kingdom.PuzzleMaps().ShowMapsDialog();
    }
    else
        Message(MP2::StringObject(obj), _("You have already been to this obelisk."), Font::BIG, Dialog::OK);
}

void ActionToTreeKnowledge(Heroes& hero, uint32_t obj, s32 dst_index)
{
    const Maps::Tiles& tile = world.GetTiles(dst_index);

    if (hero.isVisited(tile))
    {
        PlaySoundVisited;
        Message(MP2::StringObject(obj),
                _(
                    "Upon your approach, the tree opens its eyes in delight. \"It is good to see you, my student. I hope my teachings have helped you.\""
                ),
                Font::BIG, Dialog::OK);
    }
    else
    {
        const Funds& funds = tile.QuantityFunds();
        bool conditions = 0 == funds.GetValidItemsCount();
        string msg;

        // free
        if (conditions)
        {
            Sprite sprite = AGG::GetICN(ICN::EXPMRL, 4);
            msg = _(
                "Upon your approach, the tree opens its eyes in delight. \"Ahh, an adventurer! Allow me to teach you a little of what I have learned over the ages.\""
            );
            Dialog::SpriteInfo(MP2::StringObject(obj), msg, sprite);
        }
        else
        {
            const ResourceCount& rc = tile.QuantityResourceCount();

            if (hero.GetKingdom().AllowPayment(funds))
            {
                msg = _("Upon your approach, the tree opens its eyes in delight.");
                msg.append("\n");
                msg.append(
                    _(
                        "\"Ahh, an adventurer! I will be happy to teach you a little of what I have learned over the ages for a mere %{count} %{res}.\""
                    ));
                msg.append("\n");
                msg.append(_("(Just bury it around my roots.)"));
                StringReplace(msg, "%{res}", Resource::String(rc.first));
                StringReplace(msg, "%{count}", rc.second);
                auto spriteIcn = AGG::GetICN(ICN::EXPMRL, 4);
                conditions = Dialog::YES == Dialog::SpriteInfo(MP2::StringObject(obj), msg, spriteIcn,
                                                               Dialog::YES | Dialog::NO);
            }
            else
            {
                msg = _("Tears brim in the eyes of the tree.");
                msg.append("\n");
                msg.append(_("\"I need %{count} %{res}.\""));
                msg.append("\n");
                msg.append(_("it whispers. (sniff) \"Well, come back when you can pay me.\""));
                StringReplace(msg, "%{res}", Resource::String(rc.first));
                StringReplace(msg, "%{count}", rc.second);
                Message(MP2::StringObject(obj), msg, Font::BIG, Dialog::OK);
            }
        }

        if (conditions)
        {
            hero.GetKingdom().OddFundsResource(funds);
            hero.SetVisited(dst_index);
            hero.IncreaseExperience(Heroes::GetExperienceFromLevel(hero.GetLevel()) - hero.GetExperience());
        }
    }
}

void ActionToOracle(Heroes& hero, uint32_t obj, s32 dst_index)
{
    Dialog::ThievesGuild(true);
}

void ActionToDaemonCave(Heroes& hero, uint32_t obj, s32 dst_index)
{
    Maps::Tiles& tile = world.GetTiles(dst_index);

    AGG::PlayMusic(MUS::DEMONCAVE, false);

    if (Dialog::YES == Dialog::Message(MP2::StringObject(obj),
                                       _(
                                           "The entrance to the cave is dark, and a foul, sulfurous smell issues from the cave mouth. Will you enter?"
                                       ),
                                       Font::BIG, Dialog::YES | Dialog::NO))
    {
        uint32_t variant = tile.QuantityVariant();

        if (variant)
        {
            uint32_t gold = tile.QuantityGold();
            string msg;

            if (variant == 2 && hero.IsFullBagArtifacts())
                variant = 3;

            if (Dialog::YES == Dialog::Message("",
                                               _(
                                                   "You find a powerful and grotesque Demon in the cave. \"Today,\" it rasps, \"you will fight and surely die. But I will give you a choice of deaths. You may fight me, or you may fight my servants. Do you prefer to fight my servants?\""
                                               ),
                                               Font::BIG, Dialog::YES | Dialog::NO))
            {
                // battle with earth elements
                Army army(tile);
                gold = 2500;

                Battle::Result res = BattleHeroWithMonster(hero, army, dst_index);
                if (res.AttackerWins())
                {
                    hero.IncreaseExperience(res.GetExperienceAttacker());
                    msg = _("Upon defeating the daemon's servants, you find a hidden cache with %{count} gold.");
                    StringReplace(msg, "%{count}", gold);
                    DialogWithGold("", msg, gold);
                    hero.GetKingdom().AddFundsResource(Funds(Resource::GOLD, gold));
                }
                else
                {
                    BattleLose(hero, res, true);
                }
            }
                // check variants
            else if (1 == variant)
            {
                const uint32_t exp = 1000;
                msg = _(
                    "The Demon screams its challenge and attacks! After a short, desperate battle, you slay the monster and receive %{exp} experience points."
                );
                StringReplace(msg, "%{exp}", exp);
                DialogWithExp("", msg, exp);
                hero.IncreaseExperience(exp);
            }
            else if (2 == variant)
            {
                const uint32_t exp = 1000;
                const Artifact& art = tile.QuantityArtifact();
                msg = _(
                    "The Demon screams its challenge and attacks! After a short, desperate battle, you slay the monster and find the %{art} in the back of the cave."
                );
                StringReplace(msg, "%{art}", art.GetName());
                if (art.isValid()) DialogArtifactWithExp("", msg, art, exp);
                hero.PickupArtifact(art);
                hero.IncreaseExperience(exp);
            }
            else if (3 == variant)
            {
                const uint32_t exp = 1000;
                msg = _(
                    "The Demon screams its challenge and attacks! After a short, desperate battle, you slay the monster and receive %{exp) experience points and %{count} gold."
                );
                StringReplace(msg, "%{exp}", exp);
                StringReplace(msg, "%{count}", gold);
                DialogGoldWithExp("", msg, gold, exp);
                hero.IncreaseExperience(exp);
                hero.GetKingdom().AddFundsResource(Funds(Resource::GOLD, gold));
            }
            else
            {
                bool remove = true;
                Funds payment(Resource::GOLD, gold);
                Kingdom& kingdom = hero.GetKingdom();
                bool allow = kingdom.AllowPayment(payment);

                msg = allow
                          ? _(
                              "The Demon leaps upon you and has its claws at your throat before you can even draw your sword. \"Your life is mine,\" it says. \"I will sell it back to you for %{count} gold.\""
                          )
                          : _(
                              "Seeing that you do not have %{count} gold, the demon slashes you with its claws, and the last thing you see is a red haze."
                          );
                StringReplace(msg, "%{count}", gold);

                if (allow)
                {
                    if (Dialog::YES == Dialog::Message("", msg, Font::BIG, Dialog::YES | Dialog::NO))
                    {
                        remove = false;
                        kingdom.OddFundsResource(payment);
                    }
                }
                else
                    Message("", msg, Font::BIG, Dialog::OK);

                if (remove)
                {
                    Battle::Result res;
                    res.army1 = Battle::RESULT_LOSS;
                    BattleLose(hero, res, true);
                }
            }

            tile.QuantityReset();
        }
        else
            Message("", _("Except for evidence of a terrible battle, the cave is empty."), Font::BIG,
                    Dialog::OK);

        hero.SetVisited(dst_index, Visit::GLOBAL);
    }
}

void ActionToAlchemistsTower(Heroes& hero, uint32_t obj, s32 dst_index)
{
    BagArtifacts& bag = hero.GetBagArtifacts();
    uint32_t cursed = count_if(bag._items.begin(), bag._items.end(),
                               [](const Artifact& it) { return it.isAlchemistRemove(); });

    if (cursed)
    {
        payment_t payment = PaymentConditions::ForAlchemist(cursed);

        if (hero.GetKingdom().AllowPayment(payment))
        {
            string msg =
                "As you enter the Alchemist's Tower, a hobbled, graying man in a brown cloak makes his way towards you.";
            msg.append("\n");
            msg.append(_n("He checks your pack, and sees that you have 1 cursed item.",
                          "He checks your pack, and sees that you have %{count} cursed items.", cursed));
            StringReplace(msg, "%{count}", cursed);
            msg.append("\n");
            msg.append(_("For %{gold} gold, the alchemist will remove it for you. Do you pay?"));
            StringReplace(msg, "%{gold}", payment.gold);

            if (Dialog::YES == Dialog::Message("", msg, Font::BIG, Dialog::YES | Dialog::NO))
            {
                hero.GetKingdom().OddFundsResource(payment);
                bag._items.resize(distance(bag._items.begin(),
                                           remove_if(bag._items.begin(), bag._items.end(),
                                                     [](const Artifact& it) { return it.isAlchemistRemove(); })));
            }
        }
        else
            Message("",
                    _(
                        "You hear a voice from behind the locked door, \"You don't have enough gold to pay for my services.\""
                    ),
                    Font::BIG, Dialog::OK);
    }
    else
        Message("", _("You hear a voice from high above in the tower, \"Go away! I can't help you!\""),
                Font::BIG, Dialog::OK);
}

void ActionToStables(Heroes& hero, uint32_t obj, s32 dst_index)
{
    const bool cavalry = hero.GetArmy().m_troops.HasMonster(Monster::CAVALRY);
    const bool visited = hero.isVisited(obj);
    string body;

    if (!cavalry && visited)
        body = _(
            "The head groom approaches you and speaks, \"You already have a fine horse, and have no inexperienced cavalry which might make use of our trained war horses.\""
        );
    else if (!cavalry && !visited)
        body = _(
            "As you approach the stables, the head groom appears, leading a fine looking war horse. \"This steed will help speed you in your travels. Alas, his endurance will wane with a lot of heavy riding, and you must return for a fresh mount in a week. We also have many fine war horses which could benefit mounted soldiers, but you have none we can help.\""
        );
    else if (cavalry && visited)
        body = _(
            "The head groom speaks to you, \"That is a fine looking horse you have. I am afraid we can give you no better, but the horses your cavalry are riding look to be of poor breeding stock. We have many trained war horses which would aid your riders greatly. I insist you take them.\""
        );
    else if (cavalry && !visited)
        body = _(
            "As you approach the stables, the head groom appears, leading a fine looking war horse. \"This steed will help speed you in your travels. Alas, he will grow tired in a week. You must also let me give better horses to your mounted soldiers, their horses look shoddy and weak.\""
        );


    // check already visited
    if (visited)
    {
        PlaySoundVisited;
    }
    else
    {
        hero.SetVisited(dst_index);
        PlaySoundSuccess;
        hero.IncreaseMovePoints(400);
    }

    if (cavalry) hero.GetArmy().m_troops.UpgradeMonsters(Monster::CAVALRY);

    Message("", body, Font::BIG, Dialog::OK);
}

void ActionToArena(Heroes& hero, uint32_t obj, s32 dst_index)
{
    if (hero.isVisited(obj))
    {
        PlaySoundVisited;
        Message(MP2::StringObject(obj), _("The Arena guards turn you away."), Font::BIG, Dialog::OK);
    }
    else
    {
        hero.SetVisited(dst_index);
        PlaySoundSuccess;
        hero.IncreasePrimarySkill(Dialog::SelectSkillFromArena());
    }
}

void ActionToSirens(Heroes& hero, uint32_t obj, s32 dst_index)
{
    if (hero.isVisited(obj))
    {
        PlaySoundVisited;
        Message(MP2::StringObject(obj),
                _(
                    "As the sirens sing their eerie song, your small, determined army manages to overcome the urge to dive headlong into the sea."
                ),
                Font::BIG, Dialog::OK);
    }
    else
    {
        uint32_t exp = hero.GetArmy().ActionToSirens();
        string str = _(
            "You have your crew stop up their ears with wax before the sirens' eerie song has any chance of luring them to a watery grave. An eerie wailing song emanates from the sirens perched upon the rocks. Many of your crew fall under its spell, and dive into the water where they drown. You are now wiser for the visit, and gain %{exp} experience."
        );
        StringReplace(str, "%{exp}", exp);

        hero.SetVisited(dst_index);
        PlaySoundSuccess;
        Message(MP2::StringObject(obj), str, Font::BIG, Dialog::OK);
        hero.IncreaseExperience(exp);
    }
}

void ActionToJail(Heroes& hero, uint32_t obj, s32 dst_index)
{
    Kingdom& kingdom = hero.GetKingdom();

    if (kingdom.AllowRecruitHero(false, 0))
    {
        Maps::Tiles& tile = world.GetTiles(dst_index);
        PlaySoundSuccess;
        Message(MP2::StringObject(obj),
                _(
                    "In a dazzling display of daring, you break into the local jail and free the hero imprisoned there, who, in return, pledges loyalty to your cause."
                ),
                Font::BIG, Dialog::OK);

        AnimationRemoveObject(tile);
        tile.RemoveObjectSprite();
        tile.SetObject(MP2::OBJ_ZERO);

        Heroes* prisoner = world.FromJailHeroes(dst_index);

        if (prisoner)
        {
            if (prisoner->Recruit(hero.GetColor(), Maps::GetPoint(dst_index)))
                prisoner->ResetModes(Heroes::JAIL);
        }
    }
    else
    {
        string str = _(
            "You already have %{count} heroes, and regretfully must leave the prisoner in this jail to languish in agony for untold days."
        );
        StringReplace(str, "%{count}", Kingdom::GetMaxHeroes());
        PlaySoundFailure;
        Message(MP2::StringObject(obj), str, Font::BIG, Dialog::OK);
    }
}

void ActionToHutMagi(Heroes& hero, uint32_t obj, s32 dst_index)
{
    Message(MP2::StringObject(obj),
            _(
                "You enter a rickety hut and talk to the magician who lives there. He tells you of places near and far which may aid you in your journeys."
            ),
            Font::BIG, Dialog::OK);

    if (!hero.isVisited(obj, Visit::GLOBAL))
    {
        hero.SetVisited(dst_index, Visit::GLOBAL);
        world.ActionToEyeMagi(hero.GetColor());
    }
}

void ActionToEyeMagi(Heroes& hero, uint32_t obj, s32 dst_index)
{
    Message(MP2::StringObject(obj), _("This eye seems to be intently studying its surroundings."), Font::BIG,
            Dialog::OK);
}

void ActionToSphinx(Heroes& hero, uint32_t obj, s32 dst_index)
{
    Maps::Tiles& tile = world.GetTiles(dst_index);
    auto* riddle = static_cast<MapSphinx *>(world.GetMapObject(tile.GetObjectUID(obj)));

    if (!riddle || !riddle->valid)
    {
        Message(MP2::StringObject(obj),
                _("You come across a giant Sphinx. The Sphinx remains strangely quiet."), Font::BIG,
                Dialog::OK);
        return;
    }
    if (Dialog::YES !=
        Dialog::Message("",
                        _(
                            "\"I have a riddle for you,\" the Sphinx says. \"Answer correctly, and you shall be rewarded. Answer incorrectly, and you shall be eaten. Do you accept the challenge?\""
                        ),
                        Font::BIG, Dialog::YES | Dialog::NO))
    {
        return;
    }
    string header(_("The Sphinx asks you the following riddle: %{riddle}. Your answer?"));
    StringReplace(header, "%{riddle}", riddle->message);
    string answer;
    Dialog::InputString(header, answer);
    if (riddle->AnswerCorrect(answer))
    {
        const Funds& res = riddle->resources;
        const Artifact art = riddle->artifact;
        const string say = _(
            "Looking somewhat disappointed, the Sphinx sighs. You've answered my riddle so here's your reward. Now begone."
        );
        const uint32_t count = res.GetValidItemsCount();

        if (count)
        {
            if (1 == count && res.gold && art.isValid())
                DialogWithArtifactAndGold("", say, art, res.gold);
            else
            {
                Dialog::ResourceInfo("", say, res);
                if (art.isValid()) Dialog::ArtifactInfo("", say, art);
            }
        }
        else if (art.isValid()) Dialog::ArtifactInfo("", say, art);

        riddle->SetQuiet();
        hero.SetVisited(dst_index, Visit::GLOBAL);

        if (art.isValid())
            hero.PickupArtifact(art);

        if (count)
            hero.GetKingdom().AddFundsResource(res);
    }
    else
    {
        Message("",
                _(
                    "\"You guessed incorrectly,\" the Sphinx says, smiling. The Sphinx swipes at you with a paw, knocking you to the ground. Another blow makes the world go black, and you know no more."
                ),
                Font::BIG, Dialog::OK);
        Battle::Result res;
        res.army1 = Battle::RESULT_LOSS;
        BattleLose(hero, res, true);
    }
}

void ActionToBarrier(Heroes& hero, uint32_t obj, s32 dst_index)
{
    Maps::Tiles& tile = world.GetTiles(dst_index);
    Kingdom& kingdom = hero.GetKingdom();

    if (kingdom.IsVisitTravelersTent(tile.QuantityColor()))
    {
        Message(MP2::StringObject(obj),
                _(
                    "A magical barrier stands tall before you, blocking your way. Runes on the arch read,\n\"Speak the key and you may pass.\"\nAs you speak the magic word, the glowing barrier dissolves into nothingness."
                ),
                Font::BIG, Dialog::OK);

        AnimationRemoveObject(tile);
        tile.RemoveObjectSprite();
        tile.SetObject(MP2::OBJ_ZERO);
    }
    else
    {
        Message(MP2::StringObject(obj),
                _(
                    "A magical barrier stands tall before you, blocking your way. Runes on the arch read,\n\"Speak the key and you may pass.\"\nYou speak, and nothing happens."
                ),
                Font::BIG, Dialog::OK);
    }
}

void ActionToTravellersTent(Heroes& hero, uint32_t obj, s32 dst_index)
{
    Message(MP2::StringObject(obj),
            _(
                "You enter the tent and see an old woman gazing into a magic gem. She looks up and says,\n\"In my travels, I have learned much in the way of arcane magic. A great oracle taught me his skill. I have the answer you seek.\""
            ),
            Font::BIG, Dialog::OK);

    const Maps::Tiles& tile = world.GetTiles(dst_index);
    Kingdom& kingdom = hero.GetKingdom();

    kingdom.SetVisitTravelersTent(tile.QuantityColor());
}
