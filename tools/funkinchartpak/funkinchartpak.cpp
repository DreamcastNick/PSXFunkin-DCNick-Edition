/*
 * funkinchartpak by Regan "CuckyDev" Green
 * Packs Friday Night Funkin' json formatted charts into a binary file for the PSX port
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <unordered_set>

#include "json.hpp"
using json = nlohmann::json;

#define SECTION_FLAG_OPPFOCUS (1ULL << 15) // Focus on opponent
#define SECTION_FLAG_BPM_MASK 0x7FFF       // 1/24

struct Section
{
    uint64_t end;
    uint16_t flag = 0;
};

#define NOTE_FLAG_SUSTAIN (1 << 5)       // Note is a sustain note
#define NOTE_FLAG_SUSTAIN_END (1 << 6)   // Is either end of sustain
#define NOTE_FLAG_ALT_ANIM (1 << 7)      // Note plays alt animation
#define NOTE_FLAG_MINE (1 << 8)          // Note is a mine
#define NOTE_FLAG_DANGER (1 << 9)        // Note is a danger
#define NOTE_FLAG_STATIC (1 << 10)       // Note is a static
#define NOTE_FLAG_PHANTOM (1 << 11)      // Note is a phantom
#define NOTE_FLAG_POLICE (1 << 12)       // Note is a police
#define NOTE_FLAG_MAGIC (1 << 13)        // Note is a magic
#define NOTE_FLAG_HIT (1 << 14)          // Note has been hit

struct Note
{
    uint64_t pos; // 1/12 steps
    uint16_t type;
    uint8_t is_opponent, pad = 0;
};

uint16_t ChartKey(json &j)
{
    // Shaggy keys
    switch (j.value("mania", 0))
    {
    case 0:
        return 4;
    case 1:
        return 6;
    case 2:
        return 7;
    case 3:
        return 9;
    case 4:
        return 5;
    break;
    }

    // FNF original keys
    return 4;
}

uint64_t PosRound(double pos, double crochet)
{
    return static_cast<uint64_t>(std::floor(pos / crochet + 0.5));
}

void WriteWord(std::ostream &out, uint16_t word)
{
    out.put(word >> 0);
    out.put(word >> 8);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "usage: funkinchartpak in_json" << std::endl;
        return 0;
    }

    // Read json
    std::ifstream i(argv[1]);
    if (!i.is_open())
    {
        std::cout << "Failed to open " << argv[1] << std::endl;
        return 1;
    }
    json j;
    i >> j;

    auto song_info = j["song"];

    double bpm = song_info["bpm"];
    double crochet = (60.0 / bpm) * 1000.0;
    double step_crochet = crochet / 4;

    double speed = song_info["speed"];
    uint16_t keys = ChartKey(song_info);
    uint16_t max_keys = keys * 2;

    std::cout << argv[1] << " speed: " << speed << " ini bpm: " << bpm << " step_crochet: " << step_crochet << " keys: " << keys << std::endl;

    uint64_t milli_base = 0;
    uint64_t step_base = 0;

    std::vector<Section> sections;
    std::vector<Note> notes;

    uint64_t section_end = 0;
    int score = 0, dups = 0;
    std::unordered_set<uint64_t> note_fudge;
    for (auto &i : song_info["notes"]) // Iterate through sections
    {
        bool is_opponent = i["mustHitSection"] != true; // Note: swapped

        // Read section
        Section new_section;
        if (i["changeBPM"] == true)
        {
            // Update BPM (THIS IS HELL!)
            milli_base += step_crochet * (section_end - step_base);
            step_base = section_end;

            bpm = i["bpm"];
            crochet = (60.0 / bpm) * 1000.0;
            step_crochet = crochet / 4;

            std::cout << "chg bpm: " << bpm << " step_crochet: " << step_crochet << " milli_base: " << milli_base << " step_base: " << step_base << std::endl;
        }
        new_section.end = (section_end += 16) * 12; //(uint64_t)i["lengthInSteps"]) * 12; // I had to do this for compatibility
        new_section.flag = PosRound(bpm, 1.0 / 24.0) & SECTION_FLAG_BPM_MASK;
        bool is_alt = i["altAnim"] == true;
        if (is_opponent)
            new_section.flag |= SECTION_FLAG_OPPFOCUS;
        sections.push_back(new_section);

        // Read notes
        for (auto &j : i["sectionNotes"])
        {
            // Push main note
            Note new_note;
            int sustain = static_cast<int>(PosRound(j[2], step_crochet)) - 1;
            new_note.pos = (step_base * 12) + PosRound(((uint64_t)j[0] - milli_base) * 12.0, step_crochet);
            new_note.type = static_cast<uint16_t>(j[1]) % max_keys;

            new_note.is_opponent = false;

            if ((!is_opponent && new_note.type >= keys) || (is_opponent && new_note.type < keys))
                new_note.is_opponent = true;

            if (is_opponent)
                new_note.type = (new_note.type + keys) % max_keys;

            if (j[3] == "Alt Animation")
                new_note.type |= NOTE_FLAG_ALT_ANIM;

            else if ((new_note.is_opponent) && is_alt)
                new_note.type |= NOTE_FLAG_ALT_ANIM;

            if (sustain >= 0)
                new_note.type |= NOTE_FLAG_SUSTAIN_END;

            if (j[3] == "Hurt Note")
                new_note.type |= NOTE_FLAG_MINE;

            if (j[3] == "HD Note")
                new_note.type |= NOTE_FLAG_DANGER;

            if (j[3] == "Static Note")
                new_note.type |= NOTE_FLAG_STATIC;

            if (j[3] == "Phantom Note")
                new_note.type |= NOTE_FLAG_PHANTOM;

            if (j[3] == "police")
                new_note.type |= NOTE_FLAG_POLICE;

            if (j[3] == "magic")
                new_note.type |= NOTE_FLAG_MAGIC;

            // if (note_fudge.count(*((uint64_t*)&new_note)))
            //{
            //    dups += 1;
            //    continue;
            //}
            //note_fudge.insert(*((uint64_t*)&new_note));

            notes.push_back(new_note);
            if (!new_note.is_opponent)
                score += 350;

            // Push sustain notes
            for (int k = 0; k <= sustain; k++)
            {
                Note sus_note; // jerma
                sus_note.pos = new_note.pos + ((k + 1) * 12);
                sus_note.type = new_note.type | NOTE_FLAG_SUSTAIN;
                sus_note.is_opponent = new_note.is_opponent;

                if (k != sustain)
                    sus_note.type &= ~NOTE_FLAG_SUSTAIN_END;
                notes.push_back(sus_note);
            }
        }
    }
    std::cout << "max score: " << score << " dups excluded: " << dups << std::endl;

    // Sort notes
    std::sort(notes.begin(), notes.end(), [](Note a, Note b) {
        if (a.pos == b.pos)
            return (b.type & NOTE_FLAG_SUSTAIN) && !(a.type & NOTE_FLAG_SUSTAIN);
        else
            return a.pos < b.pos;
    });

    // Push dummy section and note
    Section dum_section;
    dum_section.end = 0xFFFFFFFFFFFFFFFFULL; // Changed to use a larger value
    dum_section.flag = sections[sections.size() - 1].flag;
    sections.push_back(dum_section);

    Note dum_note;
    dum_note.pos = 0xFFFFFFFFFFFFFFFFULL; // Changed to use a larger value
    dum_note.type = NOTE_FLAG_HIT;
    dum_note.is_opponent = false;
    notes.push_back(dum_note);

    // Write to output
    std::ofstream out(std::string(argv[1]) + ".cht", std::ostream::binary);
    if (!out.is_open())
    {
        std::cout << "Failed to open " << argv[1] << ".cht" << std::endl;
        return 1;
    }

    // Write headers
    WriteWord(out, keys);
    WriteWord(out, 4 + (sections.size() << 2));

    // Write sections
    for (auto &i : sections)
    {
        WriteWord(out, static_cast<uint16_t>(i.end));
        WriteWord(out, i.flag);
    }

    // Write notes
    for (auto &i : notes)
    {
        WriteWord(out, static_cast<uint16_t>(i.pos));
        WriteWord(out, i.type);
        out.put(i.is_opponent);
        out.put(0);
    }
    return 0;
}