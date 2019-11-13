function initCanvas() {
    const unit = 25;
    const current = { x: 0, y: 0 };
    const poly = [];

    const out = document.getElementById('output');

    const canvas = document.getElementById('canvas');
    const ctx = canvas.getContext('2d');
    const division = Math.floor(500 / unit);
    redraw();

    canvas.addEventListener('mouseup', onClick);
    canvas.addEventListener('mousemove', onMove);

    function onClick(e) {
        e.preventDefault();
        if (e.button === 0) {
            poly.push(Object.assign({}, current));
        } else {
            poly.pop();
        }
        redraw();
    }

    function onMove(e) {
        const x = Math.round(e.clientX / unit) * unit;
        const y = Math.round(e.clientY / unit) * unit;
        current.x = x;
        current.y = y;
        redraw();
    }

    function redraw() {
        clear();
        drawGrid();
        drawPoly();
        drawCurrent();

        out.textContent = poly.map(p => `${p.x} ${500 - p.y}`).join(' ');
        out.textContent += ' | sp svg 0.25 >plot.svg';
    }

    function clear() {
        ctx.fillStyle = 'white';
        ctx.fillRect(0, 0, 500, 500);
    }

    function drawGrid() {
        ctx.strokeStyle = 'grey';
        ctx.beginPath();
        for (let x = 0; x < division; x++) {
            ctx.moveTo(x * unit, 0);
            ctx.lineTo(x * unit, 500);
        }
        for (let y = 0; y < division; y++) {
            ctx.moveTo(0, y * unit);
            ctx.lineTo(500, y * unit);
        }
        ctx.stroke();
        ctx.closePath();
    }

    function drawCurrent() {
        ctx.fillStyle = 'red';
        ctx.fillRect(current.x - 2, current.y - 2, 4, 4);
    }

    function drawPoly() {
        if (poly.length == 0) return;

        ctx.strokeStyle = 'black';
        ctx.beginPath();
        ctx.moveTo(poly[0].x, poly[0].y);
        ctx.fillStyle = 'black';
        ctx.fillRect(poly[0].x - 2, poly[0].y - 2, 4, 4);

        ctx.fillStyle = 'blue';
        poly.slice(1).forEach(p => {
            ctx.fillRect(p.x - 2, p.y - 2, 4, 4);
            ctx.lineTo(p.x, p.y);
        });

        ctx.fillStyle = 'yellow';
        ctx.fillRect(poly[poly.length - 1].x - 2, poly[poly.length - 1].y - 2, 4, 4);

        ctx.closePath();
        ctx.stroke();
    }
}

initCanvas();