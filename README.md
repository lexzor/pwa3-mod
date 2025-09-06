## Pwn Adventure 3: Pwnie Island Modding

This project is intended to provides a framework for modding the original game. It has been created in educational purpose to improve reverse engineering and C++ skills.
Currently, only the **Windows version** is supported.

---

## How to Use

We modify the original `GameLogic.dll`. To make your custom DLL work, you need to:

1. Rename the original `GameLogic.dll` to `GameLogic.original.dll`.
2. Replace it with your built DLL from this project.

---

## Configuration

The `CMakeLists.txt` in this repository serves as a **template**. You may need to modify it before committing or creating pull requests.

To prevent Git from tracking your local changes to `CMakeLists.txt`, run:

```bash
git update-index --skip-worktree CMakeLists.txt
```

## Development

For easier development, you can set the `GAME_PATH` in `CMakeLists.txt` to point to your game executable.

This allows the DLL to be automatically copied into the game directory after building.
