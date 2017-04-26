function reversi_turn_red_em (id) {
  x=document.getElementById(id);
  x.innerHTML = "<div class=\"reversi-map-red-em\"></div>";
}
function reversi_turn_red (id) {
  x=document.getElementById(id);
  x.innerHTML = "<div class=\"reversi-map-red\"></div>";
}
function reversi_turn_blue (id) {
  x=document.getElementById(id);
  x.innerHTML = "<div class=\"reversi-map-blue\"></div>";
}
function reversi_turn_none (id) {
  x=document.getElementById(id);
  x.innerHTML = " ";
}
function set_right_point (point) {
  x=document.getElementById("reversi-point-right");
  x.innerHTML ="<span class=\"reversi-point-anime\">"+ point+"</span><br><hr class=\"reversi-point-hr border-gray\">";
}
function set_left_point (point) {
  x=document.getElementById("reversi-point-left");
  x.innerHTML ="<span class=\"reversi-point-anime\">"+ point+"</span><br><hr class=\"reversi-point-hr border-gray\">";
}
function draw_map (key) {
  for (var x=0; x<8; x++) {
    for (var y=0; y<8; y++) {
      switch (key[x*8+y]) {
        case 'A':
          reversi_turn_blue("r"+(y+1)+"c"+(x+1));
          break;
        case 'B':
          reversi_turn_red("r"+(y+1)+"c"+(x+1));
          break;
        case 'N':
          reversi_turn_none("r"+(y+1)+"c"+(x+1));
          break;
      }
    }
  }
}
function drop (x, y) {
  coverer=document.getElementById("coverer");
  coverer.classList.remove("style-hidden");
  coverer_loader=document.getElementById("coverer-loader");
  coverer_loader.classList.remove("style-hidden");
  var get = new XMLHttpRequest();
  get.open('GET', "/reversi/response?key="+mykey+"&x="+(x-1)+"&y="+(y-1), true);
  get.send();
  get.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
       if (this.responseText[0]!='{')
       {
         setTimeout(function(){coverer.classList.add("style-hidden");coverer_loader.classList.add("style-hidden");}, 500);
         return 0;
       }else {
         var response = JSON.parse(this.responseText);
         mykey=response.key1;
         set_left_point(response.p1_1);
         set_right_point(response.p2_1);
         draw_map(mykey);
         mykey=response.key2;
         setTimeout(function() {
           set_left_point(response.p1_2);
           set_right_point(response.p2_2);
           draw_map(mykey);
           if (response.x!=-1) {
             reversi_turn_red_em("r"+(parseInt(response.y)+1)+"c"+(parseInt(response.x)+1));
           }
         }, 2000);
         setTimeout(function(){coverer.classList.add("style-hidden");coverer_loader.classList.add("style-hidden");}, 2000);
       }
    }
    else if(this.readyState == 4) {
      setTimeout(function(){coverer.classList.add("style-hidden");coverer_loader.classList.add("style-hidden");}, 500);
    }
  }
  draw_map(mykey);

}
var mykey="NNNNNNNNNNNNNNNNNNNNNNNNNNNABNNNNNNBANNNNNNNNNNNNNNNNNNNNNNNNNNN";
//initailize
function initailize () {

  draw_map (mykey);
  set_right_point(2);
  set_left_point(2);
  coverer=document.getElementById("coverer");
  coverer_loader=document.getElementById("coverer-loader");
  coverer.classList.add("style-hidden");
  coverer_loader.classList.add("style-hidden");
}
