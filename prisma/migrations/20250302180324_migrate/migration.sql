/*
  Warnings:

  - You are about to drop the column `activationLink` on the `User` table. All the data in the column will be lost.
  - A unique constraint covering the columns `[activationCode]` on the table `User` will be added. If there are existing duplicate values, this will fail.
  - Added the required column `activationCode` to the `User` table without a default value. This is not possible if the table is not empty.

*/
-- DropIndex
DROP INDEX "User_activationLink_key";

-- AlterTable
ALTER TABLE "User" DROP COLUMN "activationLink",
ADD COLUMN     "activationCode" TEXT NOT NULL;

-- CreateIndex
CREATE UNIQUE INDEX "User_activationCode_key" ON "User"("activationCode");
