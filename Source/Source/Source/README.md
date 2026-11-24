# EC HEAT - Saturator by EC AUDIO

EC HEAT es un saturador analógico de alta calidad con estética azul de EC AUDIO.

## Features
- **Saturation Type A-E:** A = Tape Suave, B = Tubo, C = Warm Tape, D = Transistor, E = Hard Clip
- **DRIVE:** 0 - 100% - Knob gigante central con glow azul
- **MIX:** 0 - 100% - Mezcla Dry / Wet
- **TONE:** -12dB a +12dB
- **OUTPUT:** -24dB a +12dB
- **OVERSAMPLE:** 1x / 2x / 4x / 8x
- **ANALOG WARM:** Color analógico extra
- **SAFE CLIP:** Protección contra clipping duro
- **BYPASS:** Bypass real
- **VU Meters:** IN / OUT en tiempo real

## Estética
Panel azul metálico oscuro, con acentos rojos solo en el botón seleccionado y en los toggles. Logo EC AUDIO arriba, EC HEAT en azul claro.

## Cómo compilar

### GitHub Actions (Recomendado)
1. Sube este repo a GitHub
2. Ve a **Actions** -> Espera 2-3 min
3. Descarga el Artifact: `EC-HEAT-macOS-VST3-AU` o `EC-HEAT-Windows-VST3`

### Local en macOS
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
