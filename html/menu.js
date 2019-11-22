//only used for the slide-out nav, not integral to the msgTools project

function toggle(openID, targetID){
    let openBtn = document.getElementById(openID);
    let menu = document.getElementById(targetID);

    openBtn.addEventListener('click', function(){
        console.log(menu.classList);
        if(menu.classList.contains('closed')){
            menu.classList.remove('closed');
            menu.classList.add('open');
        } else {
            menu.classList.remove('open');
            menu.classList.add('closed');
        }
    });

}
