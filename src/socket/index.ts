import type { Server } from "socket.io";
import { randomNumber } from "../utils/random.utils";
import { logger } from "../utils/logger.utils";

type Color = "black" | "white"
type PartyUser = [string, Color, string]
type Party = [PartyUser, PartyUser]

const colors: [Color, Color] = ["black", "white"]
let waiting: Array<[string, string]> = []
let parties: Array<Party> = []

export const loadIoListeners = (io: Server) => {
  const userLeave = (id: string) => {
    waiting = waiting.filter(i => i[1] != id)
    let username: string | undefined

    parties = parties.filter(party => {
      const userExists = party.some(user => user[2] === id)
      if (userExists) {
        username = party.find(user => user[2] === id)?.[0]
        return false
      }

      return true
    })

    if (username) {
      io.emit("opponentLose", username);
    }
  }

  io.on("connection", (socket) => {
    logger.info(`user ${socket.id} was connected`)

    socket.on("searchOpponent", (username: string) => {
      waiting.push([username, socket.id])

      const opponents = waiting.filter(i => i[1] != socket.id)
      const opponent = opponents[0]
      if (opponent) {
        waiting = waiting.filter(i => i[1] != opponent[1])
        waiting = waiting.filter(i => i[1] != socket.id)

        const colorIndex = randomNumber(0, 1);
        const color = colors[colorIndex]
        const opponentColor: Color = color === "white" ? "black" : "white"

        const data: Party = [
          [username, color, socket.id],
          [opponent[0], opponentColor, opponent[1]]
        ];
        parties.push(data)

        return io.emit("findOpponent", data)
      }
    })

    socket.on("move", (data: string) => {
      const args = data.split(" ")
      const username = args[0]
      const coordinates = args[1]
      const party = parties.find(party =>
        party[0][0] === username || party[1][0] === username
      )

      if (party) {
        io.emit("moveReceive", [username, coordinates])
      }
    })

    socket.on("lose", () => {
      userLeave(socket.id)
    })

    socket.on("disconnect", () => {
      logger.info(`user ${socket.id} was disconnected`)
      userLeave(socket.id)
    })
  })
}
