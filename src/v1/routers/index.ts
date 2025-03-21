import { Router } from "express";
import { statusRouter } from "./status.router";
import { authRouter } from "./user.router";

export const v1 = Router()

v1.use("/auth", authRouter)
v1.use("/status", statusRouter)
