var connection = new WebSocket('ws://' + "192.168.4.1" + ':81/', ['arduino']);
connection.onopen = function () {
    connection.send('Connect ' + new Date());
};
connection.onmessage = function (e) {
    console.log('Server: ', e.data);
};

function send(x, y, speed, angle) {
    var data = { "x": x, "y": y, "speed": speed, "angle": angle };
    data = JSON.stringify(data);
    console.log(data);
    connection.send(data);
}
var canvas, ctx;

window.addEventListener('load', () => {
    canvas = document.getElementById('canvas');
    ctx = canvas.getContext('2d');
    resize();

    document.addEventListener('mousedown', startDrawing);
    document.addEventListener('mouseup', stopDrawing);
    document.addEventListener('mousemove', Draw);
    document.addEventListener('touchstart', startDrawing);
    document.addEventListener('touchend', stopDrawing);
    document.addEventListener('touchcancel', stopDrawing);
    document.addEventListener('touchmove', Draw);
    window.addEventListener('resize', resize);
    document.getElementById("x_coordinate").innerText = 0;
    document.getElementById("y_coordinate").innerText = 0;
    document.getElementById("speed").innerText = 0;
    document.getElementById("angle").innerText = 0;
});




var width, height, radius, x_orig, y_orig;
function resize() {
    width = window.innerWidth;
    radius = 200;
    height = radius * 6.5;
    ctx.canvas.width = width;
    ctx.canvas.height = height;
    background();
    joystick(width / 3, height / 3);
    attack('#0000FF');
}

function background() {
    document.body.style.backgroundColor = 'yellow';
    x_orig = width / 3;
    y_orig = height / 3;
    x_attack = width * 2 / 3;
    y_attack = height * 22 / 30;   
    ctx.beginPath();    
    ctx.fillStyle = '#CCCC00';
    ctx.fillRect(0,0, width, height);
    ctx.arc(x_orig, y_orig, radius + 20, 0, Math.PI * 2, true);
    ctx.fillStyle = '#ECE5E5';
    ctx.fill();
}

function joystick(width, height) {
    ctx.beginPath();
    ctx.arc(width, height, radius, 0, Math.PI * 2, true);
    ctx.fillStyle = 'blue';
    ctx.fill();
    ctx.strokeStyle = 'darkblue'; 
    ctx.lineWidth = 8;
    ctx.stroke();
}
function attack(color) {
    ctx.beginPath();
    ctx.arc(width * 2 / 3, height * 22 / 30, radius, 0, Math.PI * 2, true);
    ctx.fillStyle = color;
    ctx.fill();
    ctx.font = "100px Impact";
    ctx.fillStyle = '#FFFFFF';
    ctx.fillText("ATTACK", width * 2 / 3 - 145, height * 22 / 30 + 35);
}

let coord = { x: 0, y: 0 };
let paint = false;

function getPosition(event) {
    var mouse_x = event.clientX || event.touches[0].clientX;
    var mouse_y = event.clientY || event.touches[0].clientY;
    coord.x = mouse_x - canvas.offsetLeft;
    coord.y = mouse_y - canvas.offsetTop;
}

function is_it_in_the_circle(x, y) {
    var current_radius = Math.sqrt(Math.pow(coord.x - x, 2) + Math.pow(coord.y - y, 2));
    if (radius >= current_radius) return true
    else return false
    }



function startDrawing(event) {
    paint = true;
    getPosition(event);
    if (is_it_in_the_circle(x_orig, y_orig)) {
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        background();
        attack('#2727d6');
        joystick(coord.x, coord.y);
        flag = true;
        Draw();
    }
    else {
        flag = false;
        if (is_it_in_the_circle(x_attack, y_attack)) {
            attack('#add8e6');

        }
        else {
            attack('#0000FF');
        }
    }

}


function stopDrawing() {
    paint = false;
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    background();
    attack('#0000FF');
    joystick(width / 3, height / 3);
    document.getElementById("x_coordinate").innerText = 0;
    document.getElementById("y_coordinate").innerText = 0;
    document.getElementById("speed").innerText = 0;
    document.getElementById("angle").innerText = 0;

}

function Draw(event) {
    if (paint) {
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        background();
        attack('#0000FF');
        var angle_in_degrees, x, y, speed;
        var angle = Math.atan2((coord.y - y_orig), (coord.x - x_orig));
        if (Math.sign(angle) == -1) {
            angle_in_degrees = Math.round(-angle * 180 / Math.PI);
        }
        else {
            angle_in_degrees = Math.round(360 - angle * 180 / Math.PI);
        }


        if (is_it_in_the_circle(x_orig, y_orig)) {
            if (flag) {
                joystick(coord.x, coord.y);
                x = coord.x;
                y = coord.y;
                getPosition(event);
                var speed = Math.round(Math.abs((y - y_orig) / 2));
                var x_relative = Math.round(x - x_orig);
                var y_relative = Math.round(y - y_orig);
                document.getElementById("x_coordinate").innerText = x_relative;
                document.getElementById("y_coordinate").innerText = y_relative;
                document.getElementById("speed").innerText = speed;
                document.getElementById("angle").innerText = angle_in_degrees;

                send(x_relative, y_relative, speed, angle_in_degrees);
            }
            else {
                joystick(width / 3, height / 3);
                send(0, 0, 0, 0);
            }

            x = coord.x;
            y = coord.y;
        }
        else {
            x = radius * Math.cos(angle) + x_orig;
            y = radius * Math.sin(angle) + y_orig;
            if (flag) {
                joystick(x, y);
                getPosition(event);
                var speed = Math.round(Math.abs((y - y_orig) / 2));
                var x_relative = Math.round(x - x_orig);
                var y_relative = Math.round(y - y_orig);
                document.getElementById("x_coordinate").innerText = x_relative;
                document.getElementById("y_coordinate").innerText = y_relative;
                document.getElementById("speed").innerText = speed;
                document.getElementById("angle").innerText = angle_in_degrees;
                send(x_relative, y_relative, speed, angle_in_degrees);
            } else {
                joystick(width / 3, height / 3);
                send(0, 0, 0, 0);
            }
                
        }           
    }
} 
