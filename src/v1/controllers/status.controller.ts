import type { NextFunction, Request, Response } from "express";
import { StatusService } from "../services/status.service";

const statusService = new StatusService()

export class StatusController {
  public async status(req: Request, res: Response, next: NextFunction) {
    try {
      const status = await statusService.status()

      res.json({ status })
    } catch (err) {
      next(err)
    }
  }
}
