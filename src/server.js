import express from "express";
import morgan from "morgan";
import rootRouter from "./routers/rootRouter";

const app = express();

const logger = morgan("dev");

app.use(express.static("src/views"));
app.use(express.static("resources"));
app.use(logger);
app.use(express.urlencoded({ extended: true }));
app.use("/", rootRouter);

export default app;