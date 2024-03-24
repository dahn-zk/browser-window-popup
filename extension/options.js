async function optionsSet() {
    await browser.storage.local.set({
        params: document.querySelector("#params-input").value,
        width: document.querySelector("#width-input").valueAsNumber,
        height: document.querySelector("#height-input").valueAsNumber,
    });
}

function uiUpdate(storage) {
    document.querySelector("#params-input").value = storage.params;
    document.querySelector("#width-input").value = storage.width;
    document.querySelector("#height-input").value = storage.height;
}

browser.storage.local.get().then(uiUpdate, console.error);

document.querySelector("#save-button")
    .addEventListener("click", optionsSet);
