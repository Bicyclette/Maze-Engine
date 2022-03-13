#ifndef EDITOR_UI
#define EDITOR_UI

struct EDITOR_UI_SETTINGS
{
    int shadows{1};
    int bloom{1};
    int AO{1};
    int AOSamples{32};
    float AORadius{1.0f};
    int tone_mapping{1};
    int volumetrics{0};
    float tau{0.35f};
    float phi{1.0f};
    float fog_gain{3.0f};
    int motion_blur{1};
    int show_physics{0};
};

#endif
