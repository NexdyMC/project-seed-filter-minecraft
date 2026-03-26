static inline uint32_t getBiomeColor(int biome)
{
    switch (biome)
    {
        /* ================= OCEAN ================= */
        case ocean:
        case warm_ocean:
            return 0x000070;

        case lukewarm_ocean:
            return 0x000090;

        case cold_ocean:
            return 0x202070;

        case frozen_ocean:
            return 0xA0A0FF;

        case deep_ocean:
        case deep_lukewarm_ocean:
            return 0x000030;

        case deep_cold_ocean:
            return 0x101040;

        case deep_frozen_ocean:
            return 0x5050A0;

        /* ================= RIVER ================= */
        case river:
            return 0x0000FF;

        case frozen_river:
            return 0xA0A0FF;

        /* ================= SNOW / ICE ================= */
        case snowy_plains:
        case snowy_taiga:
            return 0xFFFFFF;

        case snowy_beach:
            return 0xDDEEFF;

        case ice_spikes:
            return 0xB0E0FF;

        /* ================= DESERT ================= */
        case desert:
        case desert_hills:
            return 0xFA9418;

        /* ================= BADLANDS ================= */
        case badlands:
        case wooded_badlands:
            return 0xD94515;

        case eroded_badlands:
            return 0xFF6F3F;

        /* ================= PLAINS / FOREST ================= */
        case plains:
            return 0x8DB360;

        case forest:
        case birch_forest:
            return 0x056621;

        case dark_forest:
            return 0x40511A;

        /* ================= SWAMP ================= */
        case swamp:
            return 0x6A7039;

        /* ================= JUNGLE ================= */
        case jungle:
        case bamboo_jungle:
            return 0x537B09;

        /* ================= SAVANNA ================= */
        case savanna:
        case savanna_plateau:
            return 0xC9D29B;

        /* ================= BEACH ================= */
        case beach:
            return 0xFADE55;

        case stony_shore:
            return 0xA2A284;

        /* ================= MOUNTAINS ================= */
        case mountains:
        case wooded_mountains:
            return 0x7F8F7F;

        case stony_peaks:
            return 0xAAAAAA;

        /* ================= MUSHROOM ================= */
        case mushroom_fields:
            return 0x7A1E3A;

        case mushroom_field_shore:
            return 0x8B3A62;

        /* ================= DEFAULT ================= */
        default:
            return 0x7BA331; // fallback
    }
}
