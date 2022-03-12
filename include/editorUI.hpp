#ifndef EDITOR_UI
#define EDITOR_UI

struct EDITOR_UI_SETTINGS
{
    int shadows{1};
    int bloom{1};
    int AO{1};
    int tone_mapping{1};
    int volumetrics{1};
    float tau{0.3f};
    float phi{2.0f};
    float fog_gain{3.0f};
    int motion_blur{1};
    int show_physics{0};
};

#endif
