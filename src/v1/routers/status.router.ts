import { Router } from "express";
import { StatusController } from "../controllers/status.controller";

export const statusRouter = Router()
const statusController = new StatusController()

statusRouter.get("/", statusController.status)
