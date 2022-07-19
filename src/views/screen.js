const view = document.getElementById("occViewerCanvas");
const fullScreenBtn = document.getElementById("fullscreen");

const handleFullscreen = () => {
    view.requestFullscreen();
};

fullScreenBtn.addEventListener("click", handleFullscreen);

download_img = function(el) {
    // get image URI from canvas object
    let canvas = document.getElementById('occViewerCanvas');

    var imageURI = canvas.toDataURL("image/jpg");
    el.href = imageURI;
};