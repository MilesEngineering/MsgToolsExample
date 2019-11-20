//only used for the slide-out nav, not integral to the msgTools project

function rightMenu(openID, closeID, targetID){
    let openBtn = document.getElementById(openID);
    let closeBtn = document.getElementById(closeID);
    let menu = document.getElementById(targetID);

    openBtn.addEventListener('click', function(){
        menu.classList.remove('closed');
        menu.classList.add('open');
    });

    closeBtn.addEventListener('click', function(){
        menu.classList.remove('open');
        menu.classList.add('closed');
    });
}
