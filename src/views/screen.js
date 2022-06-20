const view = document.getElementById("occViewerCanvas");
const fullScreenBtn = document.getElementById("fullscreen");

const handleFullscreen = () => {
    view.requestFullscreen();
};

fullScreenBtn.addEventListener("click", handleFullscreen);
