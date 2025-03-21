import dotenv from "dotenv"
import { Variables } from "./env/variables.env"

const mode_index = process.argv.indexOf("--mode")
const mode = process.argv[mode_index + 1]
const env = mode_index != -1 ? mode + "." : ""

dotenv.config({
  path: `.${env}env`
})

Variables.initialize()

import { createServer } from "node:http"
import { Server } from "socket.io"
import errorMiddleware from "./middlewares/error.middleware"
import { v1 } from "./v1/routers"
import { loadIoListeners } from "./socket"
import { prisma } from "./prisma"
import cors from "cors"
import cookieParser from "cookie-parser"
import { logger } from "./utils/logger.utils"
import express from "express"

const app = express()
const server = createServer(app)
const io = new Server(server)

loadIoListeners(io)

app.use(express.urlencoded({ extended: true }))
app.use(express.json())
app.use(cookieParser())
app.use(cors({ credentials: true }))
app.use("/api/v1/", v1)
app.use(errorMiddleware as any)

const PORT = process.env.PORT || 3000

const start = async () => {
  try {
    await prisma.$connect().then(() => logger.info("postgreSQL connected"))
    server.listen(PORT, () => {
      logger.info(`server running at ${mode} mode`)
    })
  } catch (err) {
    await prisma.$disconnect()
  }
}

start()
