export const home = (req, res) => {
    return res.sendFile(process.cwd() + "/src/views/occt-webgl-sample.html");
}