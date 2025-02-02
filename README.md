# ParametricParticles
[![Build status](https://github.com/tx00100xt/SE1-ParametricParticles/actions/workflows/cibuild.yml/badge.svg)](https://github.com/tx00100xt/SE1-ParametricParticles/actions/)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/tx00100xt/SE1-ParametricParticles)](https://github.com/tx00100xt/SE1-ParametricParticles/releases/tag/v1.2.1)
[![Github downloads](https://img.shields.io/github/downloads/tx00100xt/SE1-ParametricParticles/total.svg?logo=github&logoColor=white&style=flat-square&color=E75776)](https://github.com/tx00100xt/SE1-ParametricParticles/releases/)

Advanced particle entity for the *Serious Sam - The Second Encounter 1.07 / 1.10*  

Version 1.10 required https://github.com/tx00100xt/SeriousSamClassic or https://github.com/tx00100xt/SeriousSamClassic-VK to run.

Building Serious Sam Parametric Particles
-----------------------------------------

### Linux

Type this in your terminal:

```
git clone https://github.com/tx00100xt/SE1-ParametricParticles.git
cd SE1-ParametricParticles/Sources
mkdir build && cd build
cmake ..
make -j4
make install
```
After that , libraries will be collected in the Bin directory .   
Copy them to SeriousSamClassic/SamTSE/Bin folder.  
After that , copy Resources/*.gro to SeriousSamClassic/SamTSE folder.

Windows
-------
* This project can be compiled starting from Windows 7 and higher.

1. Download and Install [Visual Studio 2015 Community Edition] or higher.
2. Download and Install [Windows 10 SDK 10.0.14393.795] or other.
3. Open the solution in the Sources folder, select Release x64 or Release Win32 and compile it.

Supported Architectures
----------------------
* `x86`

Supported OS
-----------
* `Linux`
* `Windows`

### Build status
|CI|Platform|Compiler|Configurations|Platforms|Status|
|---|---|---|---|---|---|
|GitHub Actions|Windows, Ubuntu, FreeBSD, macOS|MSVC, GCC, Clang|Release|x86, x64|![GitHub Actions Build Status](https://github.com/tx00100xt/SE1-ParametricParticles/actions/workflows/cibuild.yml/badge.svg)

You can download a the automatically build based on the latest commit.  
To do this, go to the [Actions tab], select the top workflows, and then Artifacts.

## Image

<img width="322" height="258" align="left" src="./Images/Particles_Talos.gif">
<img width="322" height="258" align="left" src="./Images/Particles_Rain.gif">
<img width="322" height="258" align="left" src="./Images/Particles_FireSmoke.gif">
<img width="322" height="258" align="left" src="./Images/Particles_TreeBirds.gif">
<img width="322" height="258" align="left" src="./Images/Particles_Beams.gif">
<img width="322" height="258" src="./Images/Particles_MovingShower.gif">

## Video Demonstration (YouTube, clickable)
[![Parametric Particles Demo](https://img.youtube.com/vi/DcomwTeyTDI/maxresdefault.jpg)](https://youtu.be/DcomwTeyTDI)

---

## 1.1 Update Feature Showcase (YouTube, clickable)
[![1.1 Update Feature Showcase](https://img.youtube.com/vi/vHMXuQAiwK0/maxresdefault.jpg)](https://youtu.be/vHMXuQAiwK0)

---

1. [Introduction](#intro)
2. [Installation](#install)
3. [New entities](#entities)
   1. [ParametricParticles](#particles)
   2. [ParticleRotation](#rotation)
   3. [ParticleVelocity](#velocity)
   4. [SpawnShapeBox](#box)
   5. [SpawnShapeCylinder](#cylinder)
   6. [SpawnShapeSphere](#sphere)
   7. [AutoHeightMap](#heightmap)

---

# Introduction<a name="intro"></a>

This is a collection of a few new entities, that are handy during mapping.

With customizable particles, you can add special effects and a chef's touch to your map!

Waterfalls, smoke, sparkles, clouds, fire, ambient particles, dusty air, fireflies and much more - your imagination is the limit!

# Installation<a name="install"></a>

1. Download an archive from the [latest release](https://github.com/SeriousAlexej/ParametricParticles/releases/latest) page.
2. Extract this archive to the root directory of *Serious Sam - The Second Encounter 1.07*
3. Launch **Serious Editor** and add all new classes to the virtual tree. Note: gif might not show all available classes.
<img src="./Images/Particles_Install.gif">


# New entities<a name="entities"></a>

Particles are implemented with 7 new entities.

<img src="./Images/Entities.png">

To quickly check them out and see some examples, you can startup a **Parametric Particles - Demo Level** level from within the game.

<img src="./Images/DemoLevel.png">

As a short intro on how to use these entities, you can watch a video below.

## Mapping Video Demonstration (YouTube, clickable)
[![Parametric Particles Mapping Demo](https://img.youtube.com/vi/gUdtqRKgqnk/maxresdefault.jpg)](https://youtu.be/gUdtqRKgqnk)

---

### ParametricParticles<a name="particles"></a>
<img src="./Images/ParametricParticlesTbn.png">

This is the main entity, that defines all parameters of the particles.

#### Gist
When this entity is **Active**, it spawnes between **Spawn count min** and **Spawn count max** particles every **Spawn interval (seconds)** seconds.
Each particle is defined by a set of properties, starting from the *Particle* word - like **Particle color**.
Initial particle position is determined by a **Spawner shape** entity, or, if not specified, is taken from the current entity's location.

#### Events

Following events can be sent to this entity:

* **Activate event** - this event activates the entity, so that new particles will be spawned.
* **Deactivate event** - this event stops new particles from spawning, but *existing* particles will continue to exist (until they expire).
* **Stop event** - this event stops new particles from spawning and kills all alive particles.

#### Properties

* **Active** - if *TRUE*, new particles are spawned each **Spawn interval (seconds)**.
* **Alive particles max count** - defines how many particles can be alive at any given moment.
* **Background** - if *TRUE*, particles will be rendered as a part of a background. This is useful when making clouds or flying birds etc.
* **Blend type** - defines the blending mode of particle's texture on screen.
* **Clipping box size X (for rendering only)** - this property defines the stretch of this entity, change it only if you experience problems with clipping (i.e. particles disappear sooner than they should when camera is turned away from them).
* **Clipping box size Y** - same as above, but for *Y* axis.
* **Clipping box size Z** - same as above, but for *Z* axis.
* **Flat type** - determines how particles are oriented relative to the viewer.
* **Height maps (chained)** - optional pointer to a chain of **AutoHeightMap** entities, that can change particle's alpha channel depending on current elevation.
* **Online Help...** - opens up this github page.
* **Orient towards velocity** - if set to *TRUE*, particle's Y+ axis will be aligned with moving direction. Further rotation from other properties (like **Particle rotation min/max** etc) will be applied on top of it.
* **Particle alpha...** - edits a graph of relative alpha value during its lifetime.
Graph editing is implemented with an external executable *Graph.exe* that is called from the *Serious Editor*.

With this utility you can edit values of various parameters in time.

If X axis is **relative time**, then this means that X=1 is the end of a particle's lifetime, X=0.75 is a 75% of particle's lifetime and so on.

If X axis is **seconds**, then X=3 is 3 seconds into particle's lifetime, etc.

Greyed out areas of a graph mean that values within them will have no impact on this property in runtime (for example, when X is < 0, then particle does not exist yet).

<img src="./Images/Graph.png">

* **Particle color base** - base color of particles. Note: alpha value here is discarded! Use **Particle alpha...** instead!
* **Particle color animation file** - animation file with color keyframes.
* **Particle color animation** - determines which animation from **Particle color animation file** will be used. Color will be multiplied with **Particle color base**, while alpha value will be taken from **Particle alpha...**.
* **Particle lifetime max** - longest possible lifetime of a particle.
* **Particle lifetime min** - shortest possible lifetime of a particle.
* **Particle placement** - if this property is set to *Relative*, then particles act as if they were parented to this entity. Only meaningful when this entity is moving (i.e. parented to a *MovingBrush* etc).
* **Particle rotation force (chained)** - optional pointer to a chain of **ParticleRotation** entities, that can change particle's rotation during its lifetime.
* **Particle rotation max** - maximum possible initial particle rotation.
* **Particle rotation min** - minimum possible initial particle rotation.
* **Particle size X max** - maximum possible initial width of a particle.
* **Particle size X min** - minimum possible initial width of a particle.
* **Particle size Y max** - maximum possible initial height of a particle.
* **Particle size Y min** - minimum possible initial height of a particle.
* **Particle size uniform** - if set to *FALSE*, then particle's X and Y sizes are chosen independently. If *TRUE*, then if X size is picked at 30% between *X min* and *X max*, then Y shall also be 30% between *Y min* and *Y max*, etc.
* **Particle stretch X...** - edits a graph of particle's Width stretch multiplier during its lifetime.
* **Particle stretch Y...** - edits a graph of particle's Height stretch multiplier during its lifetime.
* **Particle velocity force (chained)** - optional pointer to a chain of **ParticleVelocity** entities, that can change particle's velocity during its lifetime.
* **Particle visibility fall-off** - distance at which particles are no longer visible.
* **Particle visibility hot-spot** - distance at which particles start to disappear.
* **Presimulate time span** - number of seconds to presimulate particles.
* **Spawn count max** - maximum possible count of particles that are spawned each **Spawn interval (seconds)**
* **Spawn count min** - minimum possible count of particles that are spawned each **Spawn interval (seconds)**
* **Spawn interval (seconds)** - determines how often new particles are spawned. For example, a value of 2 means that new particles are spawned every 2 seconds.
* **Spawner shape (chained)** - optional pointer to a chain of entities, that define custom spawn area (**SpawnShapeBox** / **SpawnShapeCylinder** / **SpawnShapeSphere**).
* **Texture** - texture to be used for every particle.
* **Texture animation** - selection of a possible predefined texture animation.
* **Texture animation FPS** - defines how quickly texture tiles are changed, not related to **Texture animation**! Valid only if **Texture type** is set to *Animated tiles*.
* **Texture columns** - how many tiles the texture has horizontally.
* **Texture rows** - how many tiles the texture has vertically.
* **Texture tiles count** - total number of tiles. Usually this is a product of **Texture columns** and **Texture rows**, but if some last tiles are not used, you can exclude them with this property.
* **Texture type** - defines which tile of a **Texture** is rendered for particles. *Random tile* picks 1 tile and it does not change during particle's lifetime. *Animated tiles* playes back tiles from first to last (looping) using previous properties.

---

### ParticleRotation<a name="rotation"></a>
<img src="./Images/ParticlesRotationTbn.png">

This is an additional entity, representing a dynamic rotation speed, that can affect particles.

#### Gist
This entity is helpful when you want to rotate particles during their lifetime.

Multiple rotation forces can be used by bulding up a chain of **ParticleRotation** entities.

*Note - order or these entities in chain is not important, they all act simultaniously.*

#### Properties

* **Next rotation force (chained)** - a pointer to another **ParticleRotation** entity, that should also be used (order is not important, all entities act simultaniously). Loops in chains are not allowed.
* **Probability of this rotation** - probability from 0 to 1. For example, value of 0.77 means that each particle has a 77% probability of being affected by this rotation. Does not affect other rotations in the chain.
* **Rotation speed max** - maximum possible initial speed for this rotation.
* **Rotation speed min** - minimum possible initial speed for this rotation.
* **Rotation speed stretch...** - edits a graph of this rotation speed stretch factor during particle's lifetime. Can also have negative values to invert the rotation.
* **Loop speed stretch graph** - if *TRUE*, then graph of this rotation's speed stretch is looped.

---

### ParticleVelocity<a name="velocity"></a>
<img src="./Images/ParticlesVelocityTbn.png">

This is an additional entity, representing a dynamic velocity, that can affect particles. Direction of velocity is defined by this entity's orientation.

#### Gist
This entity is helpful when you want to move particles during their lifetime.

Multiple velocities can be used by building up a chain of **ParticleVelocity** entities.

*Note - order of these entities in chain is not important, they all act simultaniously.*

#### Properties

* **Next velocity force (chained)** - a pointer to another **ParticleVelocity** entity, that should also be used (order is not important, all entities act simultaniously). Loops in chains are not allowed.
* **Probability of this velocity** - probability from 0 to 1. For example, value of 0.88 means that each particle has a 88% probability of being affect by this velocity. Does not affect other velocities in the chain.
* **Velocity random heading max** - maximum possible initial heading deviation for this velocity's direction (which is defined by this entity's orientation).
* **Velocity random heading min** - minimum possible initial heading deviation for this velocity's direction (which is defined by this entity's orientation).
* **Velocity random pitch max** - maximum possible initial pitch deviation for this velocity's direction (which is defined by this entity's orientation).
* **Velocity random pitch min** - maximum possible initial pitch deviation for this velocity's direction (which is defined by this entity's orientation).
* **Velocity speed stretch...** - edits a graph of this velocity speed stretch factor during particle's lifetime. Can also have negative values to invert the velocity.
* **Loop speed stretch graph** - if *TRUE*, then graph of this velocity's speed stretch is looped.

---

### SpawnShapeBox<a name="box"></a>
<img src="./Images/SpawnShapeBoxTbn.png">

This is an additional entity, representing a box-shaped spawn volume. Particles are spawned uniformly within it.

Multiple spawn shapes of different kind can be used by building up a chain of such entities (**SpawnShapeBox** / **SpawnShapeCylinder** / **SpawnShapeSphere**).

#### Properties

* **Size X** - X size of this box.
* **Size Y** - Y size of this box.
* **Size Z** - Z size of this box.
* **Inner Size X** - X size of inner part of this box, where particles are not allowed to spawn.
* **Inner Size Y** - Y size of inner part of this box, where particles are not allowed to spawn.
* **Inner Size Z** - Z size of inner part of this box, where particles are not allowed to spawn.
* **Next spawner shape (chained)** - a pointer to another spawn shape entity, that should also be used (order is not important, particles are distributed uniformly across all spawn shapes). Loops in chains are not allowed.

---

### SpawnShapeCylinder<a name="cylinder"></a>
<img src="./Images/SpawnShapeCylinderTbn.png">

This is an additional entity, representing a cylinder-shaped spawn volume. Particles are spawned uniformly within it.

Multiple spawn shapes of different kind can be used by building up a chain of such entities (**SpawnShapeBox** / **SpawnShapeCylinder** / **SpawnShapeSphere**).

#### Properties

* **Diameter** - diameter of this cylinder.
* **Height** - height of this cylinder.
* **Inner Diameter** - diameter of inner part of this cylinder, where particles are not allowed to spawn.
* **Inner Height** - height of inner part of this cylinder, where particles are not allowed to spawn.
* **Next spawner shape (chained)** - a pointer to another spawn shape entity, that should also be used (order is not important, particles are distributed uniformly across all spawn shapes). Loops in chains are not allowed.

---

### SpawnShapeSphere<a name="sphere"></a>
<img src="./Images/SpawnShapeSphereTbn.png">

This is an additional entity, representing a sphere-shaped spawn volume. Particles are spawned uniformly within it.

Multiple spawn shapes of different kind can be used by building up a chain of such entities (**SpawnShapeBox** / **SpawnShapeCylinder** / **SpawnShapeSphere**).

#### Properties

* **Diameter** - diameter of this sphere.
* **Inner Diameter** - diameter of inner part of this sphere, where particles are not allowed to spawn.
* **Next spawner shape (chained)** - a pointer to another spawn shape entity, that should also be used (order is not important, particles are distributed uniformly across all spawn shapes). Loops in chains are not allowed.

---

### AutoHeightMap<a name="heightmap"></a>
<img src="./Images/AutoHeightMapTbn.png">

This entity automatically generates heightmap for given area (orientation can be arbitrary). Particles below heightmap area will have 0 alpha channel. Particles above and outside of heightmap area will have their alpha channel intact.

Multiple heightmaps can be used for single particle entity by building up a chain of **AutoHeightMap** entities.

#### Properties

* **Fixed** - if set to *TRUE*, size and step properties will be locked and recalculation will be forbidden.
* **Next height map (chained)** - a pointer to another **AutoHeightMap** entity, that should also be used. Loops in chains are not allowed.
* **Recalculate** - a shortcut for discarding old heightmap and calculating a new one.
* **Size X** - X size of volume for this heightmap.
* **Size Y** - Y size of volume for this heightmap.
* **Size Z** - Z size of volume for this heightmap.
* **Step in meters** - interval at which height values are sampled. Lower number means higher resolution.
* **Visible** - if set to *TRUE*, calculated heightmap is visualized in *Serious Editor* in form of transparent yellow particles.

[Visual Studio 2015 Community Edition]: https://go.microsoft.com/fwlink/?LinkId=615448&clcid=0x409 "Visual Studio 2015 Community Edition"
[Windows 10 SDK 10.0.14393.795]: https://go.microsoft.com/fwlink/p/?LinkId=838916 "Windows 10 SDK 10.0.14393.795"
[Actions tab]: https://github.com/tx00100xt/SE1-ParametricParticles/actions "Download Artifacts"
