import express from "express";
import morgan from "morgan";
import rootRouter from "./routers/rootRouter";

const app = express();

const logger = morgan("dev");

app.set("view engine", "ejs");
app.engine("html", require("ejs").renderFile);
app.set("views", process.cwd() + "/src/views");
app.use(logger);
app.use(express.urlencoded({ extended: true }));

app.use("/", rootRouter);

export default app;