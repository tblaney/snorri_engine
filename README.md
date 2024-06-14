# Custom C++ SDF Renderer using Raymarching (WIP)

This repository contains the work-in-progress code for a custom C++ game engine that utilizes Signed Distance Functions (SDFs) instead of traditional meshes, with Raymarching as the rendering technique.

## Project Overview

The goal of this project is to develop a custom game engine that fully employs SDFs for rendering complex 3D scenes. By using Raymarching, the engine calculates and renders scenes directly from distance fields, bypassing traditional polygon-based rendering methods. This approach allows for unique rendering effects and potentially more efficient handling of complex shapes and lighting phenomena.

## Current Status

- **Engine Framework**: Basic implementation of game object hierarchy and the use of json files to construct scenes.
- **Rendering Pipeline**: Beginning integration of a rendering pipeline capable of processing SDFs and producing visuals through Raymarching.

## Motivation

The motivation behind creating this custom engine is to explore the potential for SDFs to simplify content creation and enhance rendering capabilities in real-time applications. This technique is particularly appealing for its ability to naturally incorporate complex geometries and soft shadows with less computational overhead compared to traditional methods.

## Comparable Unity Project

For a reference on how SDFs are utilized in a more conventional game development environment, check out the comparable Unity project: [Monster Maker Unity SDFs](https://github.com/tblaney/monster_maker_unity_sdfs/).
