![video](https://i.imgur.com/Fg0FeTC.gif)

# Chess
A project that allows you to play chess with other people from the console. You can deploy a [server](https://github.com/f01zy/Chess/tree/server) locally and play, for example, on one computer, or use our remote server to play with other people.

## Dependencies
* git
* cmake
* postgres (to start a local server)
* bun (to start a local server)

## Instalation
### Client
To install the client, you need to clone this repository, install the dependencies, and then build the project.

```bash
git clone https://github.com/f01zy/Chess/ && cd Chess
sudo ./dependencies.sh
mkdir build
cd build
cmake ..
make
```

After these steps you will have a chess script, which is the game.

### Server
If you are going to run a local server, you need to clone the [server](https://github.com/f01zy/Chess/tree/server) source code for yourself, fill in the environment variable (rename .example.env to .development.env), and run the server.

```bash
git clone https://github.com/f01zy/Chess/ -b server && cd Chess
bun i
mv .example.env .development.env
sudo nano .development.env (fill in the environment variables)
bun run run:development
```
