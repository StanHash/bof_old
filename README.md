# Breath of Fire GBA

This is a decompilation of the GBA port/remake of Breath of Fire (specifically the european multi lingual version).

It builds the following ROM:

* **[bof.gba]** `sha1: fdc7e7b6ab680229dcd159eb4dd7d1967b9e88ee`

[bof.gba]: https://datomatic.no-intro.org/index.php?page=show_record&s=23&n=0381

## Quick setup

- get devkitARM. Other `arm-none-eabi` toolchain may work.
- get a copy of the original rom (still required for every build for now), put it in this folder and name it `baserom.gba`.
- run [tools/install-agbcc.sh](tools/install-agbcc.sh) or get agbcc another way.
- `make compare`

## Contributing

Please do. I'm going to be pretty strict on what I accept though, so mind your formatting and naming conventions.

## My Other stuff

* [**StanHash/fe6**](https://github.com/StanHash/fe6), a decompilation of Fire Emblem: The Binding Blade (JP)
* [**StanHash/fomt**](https://github.com/StanHash/fomt), a decompilation of Havest Moon: Friends of Mineral Town (US)

## Contact

You can find me over at the [Fire Emblem Universe Discord](https://feuniverse.us/t/feu-discord-server/1480?u=stanh) under the handle `nat5#4387`. I also lurk the pret Discord.
