export class Variables {
  public static readonly SMTP_HOST: string = process.env.SMTP_HOST!
  public static readonly SMTP_PORT: string = process.env.SMTP_PORT!
  public static readonly SMTP_USERNAME: string = process.env.SMTP_USERNAME!
  public static readonly SMTP_PASSWORD: string = process.env.SMTP_PASSWORD!

  public static readonly JWT_ACCESS_SECRET: string = process.env.JWT_ACCESS_SECRET!
  public static readonly JWT_REFRESH_SECRET: string = process.env.JWT_REFRESH_SECRET!

  public static readonly DATABASE_URL: string = process.env.DATABASE_URL!
  public static readonly PORT: string = process.env.PORT!
}
