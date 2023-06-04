# NYCU CA2023 Final Project
Group 39

## Dependencies

- [glfw](https://github.com/glfw/glfw)
- [glad](https://github.com/Dav1dde/glad)
- [glm](https://github.com/g-truc/glm)

### Dependencies for Windows

Visual Studio

### Visual Studio 2019

- Open `vs2019/HW2.sln`
- Select config then build (CTRL+SHIFT+B)
- Use F5 to debug or CTRL+F5 to run.

## Title
模擬樹木年輪生長
- 參考論文: [Procedural Texturing of Solid Wood with Knots](https://dl.acm.org/doi/10.1145/3528223.3530081) (2022)
- 參考GitHub Code: [procedural_knots](https://github.com/marialarsson/procedural_knots) (Python)

## Goal / Motivation
在 Procedural Texturing of Solid Wood with Knots 論文中使用 Procedural Texturing 的方法實作樹木年輪中含有節(Knots)的紋理,其中包含部分模擬年輪的生長。因此我們希望使用 Procedural Texturing 的方法來模擬樹木年輪生長的情況。

## Related work
- Texture synthesis
- Procedural modeling and texturing
- Tree growth simulation

## Proposed approach
- 定義年輪表示方法
- 莖到節之間的轉變
- 顏色的渲染
- 輸入樹木骨架和節的資訊
- 調整參數模擬樹木不同特性
- 模擬生長動畫

## Expected results
- 了解樹木內部生物構造及成長方式:"節"與年輪的相互影響
- 了解目前對於樹木如何進行建模，各種參數對於模型的意義
- 嘗試依不同種類的樹木，模擬出其特性(成長)
