
  //39 114 -99
  //39 114 514

  function getAngle(a, b) {
    const MathPI = 3.1415926535897932384626
    const dx = b.x - a.x;
    const dy = b.y - a.y;
    const dz = b.z - a.z;
    const angleXZ = Math.atan2(dz, dx) * 180 / Math.PI;
    const angleY = Math.atan2(dy, Math.sqrt(dx*dx + dz*dz)) * 180 / MathPI;
    const angle = [-angleXZ, -angleY];
    return angle;
  }
const a = {x:39, y:114, z:-99};
const b = {x:44, y:122, z:-94};
const angle = getAngle(a, b);
console.log(angle); // [-45,-48.52706569105949]

