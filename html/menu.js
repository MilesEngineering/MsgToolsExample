//only used for the slide-out nav, not integral to the msgTools project
(function toggle() {
    const selectors = document.querySelectorAll('[target]');

    for(let i = 0; i < selectors.length; i++ ){
        selectors[i].addEventListener('click', function(){
            let targetName = selectors[i].getAttribute('target');
            let target = document.getElementById(targetName);
            if(target.classList.contains('closed')){
                let openElements = document.querySelectorAll('.open');
                console.log(openElements);
                for(let j = 0; j < openElements.length; j++){
                    if( !openElements[j].classList.contains('dropdown')){
                        openElements[j].classList.remove('open');
                        openElements[j].classList.add('closed');
                    }
                }

                target.classList.remove('closed');
                target.classList.add('open');
            } else {
                target.classList.remove('open');
                target.classList.add('closed');
            }

        });
    }
})();
