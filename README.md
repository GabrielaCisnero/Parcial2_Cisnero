# Trabajo Final PEV 2 - Leaf Race
**Alumno:** Gabriela Cisnero | **Legajo:** 46885065 | **Modo:** Recolectar y Entregar | **Versión de Unreal:** UE 5.6.1

---
## Reglas y Mecánica Principal
- **Mecánica:** Dos equipos (Rojo y Azul) compiten por recolectar hojas repartidas por el mapa. Cada jugador carga una hoja a la vez y la entrega en la base de su equipo con la tecla E. También puede robar una hoja almacenada en la base rival. Las hojas recogidas reaparecen a los pocos segundos.
- **Cómo se gana:** Gana el primer equipo que llegue a 15 hojas en su base, o, al terminar los 5 minutos, el equipo con más hojas almacenadas (empate si es igual).

---
## Ficha Técnica de Red
- **RepNotify:** `bHasLeaf` en `APEV2_Character` -> muestra/oculta la hoja sobre la cabeza del jugador en todos los clientes.
- **RepNotify:** `Team` en `APEV2_PlayerState` -> actualiza el equipo y el color del personaje.
- **RepNotify:** `RedLeaves` / `BlueLeaves` / `CurrentMatchState` en `APEV2_GameState` -> actualizan el HUD (puntajes y estado de partida).
- **RepNotify:** `bAvailable` en `APEV2_Leaf` -> oculta/muestra la hoja y activa o desactiva su colisión.
- **Server RPC:** `Server_Interact` en `APEV2_Character` -> el servidor valida cercanía, equipo y estado de partida antes de recoger, entregar o robar.
- **Client RPC:** `Client_ShowEndScreen` y `Client_ShowFeedback` en `APEV2_PlayerController` -> pantalla final de victoria/derrota y mensajes de feedback solo para el jugador involucrado.
- **NetMulticast RPC:** `Multicast_PlayInteractEffect` en `APEV2_Character` -> sonido para todos los clientes al recoger, entregar o robar una hoja.

---
## Menú Principal (extra)
- Permite elegir la cantidad de jugadores (1 a 4), ingresar la IP de un host para unirse y salir del juego.
- El juego también se puede probar directamente desde el editor abriendo `L_GameArena`, como se indica abajo.

---
## Cómo Probar el Proyecto
1. Abrir el mapa: `Content/PEV2/Maps/L_GameArena`.
2. En Play (PIE): `Number of Players = 2` (o 4) y `Net Mode = Play As Listen Server`. En UE 5.6 el campo se llama `Play Number Of Clients` (Editor Preferences > Level Editor - Play).
3. Esperar la cuenta regresiva inicial (10 s), recoger hojas con `E` y entregarlas en la base del propio equipo.
