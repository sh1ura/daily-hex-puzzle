const F_SIZE = 9;
const F_CENTER = 4;

const PN = origPiece.length;

const initField = (function () {
    var n = {p:[[]], x:F_SIZE, y:F_SIZE};
    for(var y = 0; y < F_SIZE; y++) {
	n.p[y] = Array(F_SIZE).fill(0);
    }
    return n;
}) ();

const fieldMask = mask(initField);
var answer = copyObj(fieldMask);

var piece;
initPiece();

function copyObj(x) {
    return JSON.parse(JSON.stringify(x));
}

order = [];
function hintOrder() {
    for(i = 0; i < PN; i++) {
	order[i] = i;
    }
    for(var i = 0; i < 100; i++) {
	var p1 = Math.floor(Math.random() * PN);
	var p2 = Math.floor(Math.random() * PN);
	tmp = order[p1];
	order[p1] = order[p2];
	order[p2] = tmp;
    }
}

var limit = 1;
function showField(f) {
    let canvas = document.getElementById("canvas");
    let context = canvas.getContext("2d");
    context.save();
    context.clearRect(0, 0, 1000, 1000);
    if(rotate) {
	context.rotate(-Math.PI / 6);
	context.translate(-150, 100);
    }
    function drawHex(x, y, n) {
	const fillColor = [
	    "#FF0000", "#00FF00", "#0000FF",
	    "#FFFF00", "#00FFFF", "#FF00FF",
	    "#AA0000", "#00AA00", "#0000AA",
	    "#664444", "#446644",
	];
	const STEP = 50;
	var cx = STEP * (x + F_CENTER / 2 + 1) - (STEP * y / 2)
	var cy = (STEP * Math.sqrt(3) / 2) * (y + 1);
	if(n < 0) {
	    // nothing
	}
	else if(0 < n && n < "a".charCodeAt(0) && order[n - "A".charCodeAt(0)] < limit) {
	    context.beginPath();
	    context.fillStyle = fillColor[n % 11];
	    for(var i = 0; i < 6; i++) {
		var theta = i * 2 * Math.PI / 6;
		var rx = cx + Math.sin(theta) * STEP * .59;
		var ry = cy + Math.cos(theta) * STEP * .59;
		if(i == 0) {
		    context.moveTo(rx, ry);
		}
		else {
		    context.lineTo(rx, ry);
		}
	    }
	    context.closePath();
	    context.fill();
	}
	else {
	    if(n == 0 || n >= "a".charCodeAt(0)) {
		context.save();
		if(rotate) {
		    context.translate(cx, cy);
		    context.rotate(Math.PI / 6);
		    context.translate(-cx, -cy);
		}
		context.font = "20px sunserif";
		context.fillStyle = "#000000";
		context.textAlign = "center";
		context.textBaseline = "middle";
		context.fillText(map[y][x], cx, cy);
		context.restore();

	    }

	    context.beginPath();
	    for(var i = 0; i < 6; i++) {
		var theta = i * 2 * Math.PI / 6;
		var rx = cx + Math.sin(theta) * STEP * .55;
		var ry = cy + Math.cos(theta) * STEP * .55;
		if(i == 0) {
		    context.moveTo(rx, ry);
		}
		else {
		    context.lineTo(rx, ry);
		}
	    }
	    context.closePath();
	    context.stroke();
	}
    }

    for(var y = 0; y < F_SIZE; y++) {
	for(var x = 0; x < F_SIZE; x++) {
	    drawHex(x, y, f.p[y][x]);
	}
    }
    context.restore();
}

function simpleRot(f) { // just rotate the pattern CCW 60deg
    var nf = copyObj(fieldMask);
    
    for(var y = 0; y < F_SIZE; y++) {
	for(var x = 0; x < F_SIZE; x++) {
	    var x2 = y;
	    var y2 = F_CENTER - x + y;
	    if(0 <= x2 && x2 < F_SIZE && 0 <= y2 && y2 < F_SIZE) {
		nf.p[y][x] = f.p[y2][x2];
	    }
	    else {
		nf.p[y][x] = -1;
	    }
	}
    }
    return nf;
}

function mask(f) { // mask outside of the pattern
    var nf = copyObj(initField);
    
    nf.x = f.x;
    nf.y = f.y;
    for(var y = 0; y < F_SIZE; y++) {	
	for(var x = 0; x < F_SIZE; x++) {
	    var x2 = y;
	    var y2 = F_CENTER - x + y;
	    if(0 <= x2 && x2 < F_SIZE && 0 <= y2 && y2 < F_SIZE) {
		if(x < f.x && y < f.y) {
		    nf.p[y][x] = f.p[y][x];
		}
		else {
		    nf.p[y][x] = 0;
		}
	    }
	    else {
		nf.p[y][x] = -1;
	    }
	}
    }
    return nf;
}

function outofMask(f) { // check if the pattern is on the mask
    for(var y = 0; y < F_SIZE; y++) {
	for(var x = 0; x < F_SIZE; x++) {
	    if(fieldMask.p[y][x] < 0 && f.p[y][x] > 0) {
		return true;
	    }
	}
    }
    return false;
}

function transpose(f) { // mirror
    var nf = copyObj(fieldMask);
    
    for(var y = 0; y < F_SIZE; y++) {
	for(var x = 0; x < F_SIZE; x++) {
	    nf.p[y][x] = f.p[x][y];
	}
    }
    return nf;
}		       

function shiftUp(orgf) { // delete empty line
    var sum;
    var f = copyObj(orgf), f2;
    
    for(var i = 0; i < F_SIZE; i++) {
	f2 = copyObj(f);
	sum = 0;
	for(var x = 0; x < F_SIZE; x++) {
	    if(f2.p[0][x] > 0) {
		sum++;
	    }
	}
	if(sum == 0) {
	    for(var y = 1; y < F_SIZE; y++) {
		for(var x = 0; x < F_SIZE; x++) {
		    if(f2.p[y][x] >= 0) {
			f2.p[y-1][x] = f2.p[y][x];
		    }
		    else {
			f2.p[y-1][x] = 0;
		    }
		}
	    }
	    for(var x = F_CENTER; x < F_SIZE; x++) {
		f2.p[F_SIZE-1][x] = 0;
	    }
	    if(outofMask(f2)) {
		return f;
	    }
	    f = copyObj(mask(f2));
	}
    }
    return f;
}

function shiftXY(f) { // shift the pattern to upper-left
    f = shiftUp(f); 
    f = transpose(f);
    f = shiftUp(f);
    f = transpose(f); 
    return f;
}

function shiftRot(f) { // rotate and shift to upper-left
    return shiftXY(simpleRot(f));
}

function isSame(a, b) { // compare two patterns
    for(var y = 0; y < F_SIZE; y++) {
	for(var x = 0; x < F_SIZE; x++) {
	    if(a.p[y][x] != b.p[y][x]) {
		return false;
	    }
	}
    }
    return true;
}

function initPiece() {
    piece = copyObj(origPiece);
    for(var pn = 0; pn < PN; pn++) {
	piece[pn][0] = mask(piece[pn][0]);
	for(var y = 0; y < F_SIZE; y++) {
	    for(var x = 0; x < F_SIZE; x++) {
		if(piece[pn][0].p[y][x] > 0) {
		    piece[pn][0].p[y][x] = "A".charCodeAt(0) + pn;
		}
	    }
	}
    }
    shuffle();
}

function genPose() { // generate rotated and mirrored patterns of pieces
    var f, i, j, pn, pose;
    
    for(pn = 0; pn < PN; pn++) {
	if(flip == "back") {
	    piece[pn][0] = transpose(piece[pn][0]);
	}
	// generating rotated pattern
	f = copyObj(piece[pn][0]);
	for(i = 0; i < 6; i++) {
	    f = shiftRot(f);
	    for(j = 0; j < piece[pn].length; j++) {
		if(isSame(piece[pn][j], f)) {
		    break; // delete redundant pattern
		}
	    }
	    if(j == piece[pn].length) {
		piece[pn].push(copyObj(f));
	    }
	}
	if(flip == "both") {
	    // generating mirrored and rotated pattern
	    f = transpose(piece[pn][0]);
	    for(i = 0; i < 6; i++) {
		f = shiftRot(f);
		for(j = 0; j < piece[pn].length; j++) {
		    if(isSame(piece[pn][j], f)) {
			break; // delete redundant pattern
		    }
	    }
		if(j == piece[pn].length) {
		    piece[pn].push(copyObj(f));
		}
	    }
	}
    }
    // calculate the effective pattern size
    for(pn = 0; pn < PN; pn++) {
	for(pose = 0; pose < piece[pn].length; pose++) {
	    var x, y;
	    piece[pn][pose].x = 0;
	    piece[pn][pose].y = 0;
	    for(y = 0; y < F_SIZE; y++) {
		for(x = 0; x < F_SIZE; x++) {
		    if(piece[pn][pose].p[y][x] > 0) {
			if(piece[pn][pose].x <= x) {
			    piece[pn][pose].x = x + 1;
			}
			if(piece[pn][pose].y <= y) {
			    piece[pn][pose].y = y + 1;
			}
		    }
		}
	    }
	}
    }
}

// check if the pattern is placeable at (sx, sy)
function isPlaceable(f, p, sx, sy) {
    for(var y = 0; y < p.y; y++) {
	for(var x = 0; x < p.x; x++) {
	    if(p.p[y][x] > 0) {
		if(f.p[y + sy][x + sx] != 0) { // collision
		    return false;
		}
	    }
	}
    }
    return true;
}

function shuffle() {
    var tmp;

    for(var i = 0; i < 100; i++) {
	var p1 = Math.floor(Math.random() * PN);
	var p2 = Math.floor(Math.random() * PN);
	tmp = piece[p1];
	piece[p1] = piece[p2];
	piece[p2] = tmp;
    }
}

// place the pattern at (sx, sy)
function doPlace(f, p, sx, sy) {
    for(y = 0; y < p.y; y++) {
	for(x = 0; x < p.x; x++) {
	    if(p.p[y][x] > 0) {
		f.p[y + sy][x + sx] = p.p[y][x];
	    }
	}
    }
}

// recursive search of placing pieces
function placeCheck(f, used, pn) {
    var result = 0;

    //      console.log(used);
    if(pn == PN) { // solution is found
	answer = copyObj(f);
	return 1;
    }

    // find top-left empty cell
    OUT:
    for(var y = 0; y < F_SIZE; y++) {
	for(var x = 0; x < F_SIZE; x++) {
	    if(f.p[y][x] == 0) {
		break OUT;
	    }
	}
    }
    for(var i = 0; i < PN; i++) {
	if(used[i]) { // select unsed piece
	    continue;
	}
	for(var pose = 0; pose < piece[i].length; pose++) {
	    for(var py = 0; py < piece[i][pose].y; py++) {
		for(var px = 0; px < piece[i][pose].x; px++) {
		    var posx = x - px;
		    var posy = y - py;
		    if(posx < 0 || posy < 0 ||
		       posx + piece[i][pose].x > F_SIZE ||
		       posy + piece[i][pose].y > F_SIZE) {
			continue; // piece is not in the field
		    }
		    if(piece[i][pose].p[py][px] <= 0) {
			continue; // the piece must fill (y, x)
		    }
		    if(isPlaceable(f, piece[i][pose], posx, posy)) {
			var newField = copyObj(f);
			var newUsed = copyObj(used);
			newUsed[i] = 1;
			doPlace(newField, piece[i][pose], posx, posy);
			result += placeCheck(newField, newUsed, pn + 1);
			if(result) {
			    return 1;
			}
		    }
		}
	    }
	}
    }
    return result;
}

function setMark(f, txt, val) {
    for(var y = 0; y < F_SIZE; y++) {
	for(var x = 0; x < F_SIZE; x++) {
	    if(map[y][x] == txt) {
		f.p[y][x] = val;
	    }
	}
    }
}

initPiece();
genPose();
//    console.log(piece);

window.onload = function() {
    var field = copyObj(fieldMask);
    setMark(field, "NON", -1);
    showField(field);
    var today = new Date();
    document.getElementById("month").options[today.getMonth()].selected = true;
    document.getElementById("date").value = today.getDate();
    if(document.getElementById("day") != null) {
	document.getElementById("day").options[today.getDay()].selected = true;
    }
    document.getElementById("limit").max = PN;
};

function solve() {
    initPiece();
    genPose();
    var field = copyObj(fieldMask);
    var used = Array(PN).fill(0);
    answer = copyObj(fieldMask);
    setMark(answer, "NON", -1);
    setMark(field, "NON", -1);
    setMark(field, document.getElementById("month").value, "m".charCodeAt(0));
    setMark(field, document.getElementById("date").value, "d".charCodeAt(0));
    if(document.getElementById("day") != null) {
	setMark(field, document.getElementById("day").value, "w".charCodeAt(0));
    }
    placeCheck(field, used, 0);
    document.getElementById("limit").disabled = false;
    hintOrder();
    showField(answer);
}

function redraw() {
    limit = Number(document.getElementById("limit").value);
    showField(answer);
}
