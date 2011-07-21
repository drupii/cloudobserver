            var camera, scene, renderer;
        
            var fov = 70,
                texture_placeholder, isUserInteracting = false,
                onMouseDownMouseX = 0,
                onMouseDownMouseY = 0,
                lon = 90,
                onMouseDownLon = 0,
                lat = 0,
                onMouseDownLat = 0,
                phi = 0,
                theta = 0,
                html5support = 0;
        
            function dinit()
            {
        
                var container, mesh;
        
                container = $('#container');
                container.html('');
                camera = new THREE.Camera(fov, window.innerWidth / window.innerHeight, 1, 1100);
        
        
                scene = new THREE.Scene();
        
                texture_placeholder = $('<canvas></canvas>');
                texture_placeholder.width = 512;
                texture_placeholder.height = 512;
        
                var context = texture_placeholder[0].getContext('2d');
                context.fillStyle = 'rgb( 200, 200, 200 )';
                context.fillRect(0, 0, texture_placeholder.width, texture_placeholder.height);
        
                var materials = [
        
                loadTexture('textures/cube/skybox/px.jpg'), // right
                loadTexture('textures/cube/skybox/nx.jpg'), // left
                loadTexture('textures/cube/skybox/py.jpg'), // top
                loadTexture('textures/cube/skybox/ny.jpg'), // bottom
                loadTexture('textures/cube/skybox/pz.jpg'), // back
                loadTexture('textures/cube/skybox/nz.jpg') // front
                ];
        
                mesh = new THREE.Mesh(new Cube(300, 300, 300, 7, 7, materials, true), new THREE.MeshFaceMaterial());
                mesh.overdraw = true;
                scene.addObject(mesh);
        
                renderer = new THREE.CanvasRenderer();
                renderer.setSize(window.innerWidth, window.innerHeight);
        
                container.append(renderer.domElement);
        
                document.addEventListener('mousedown', onDocumentMouseDown, false);
                document.addEventListener('mousemove', onDocumentMouseMove, false);
                document.addEventListener('mouseup', onDocumentMouseUp, false);
                document.addEventListener('mousewheel', onDocumentMouseWheel, false);
        
                document.addEventListener('touchstart', onDocumentTouchStart, false);
                document.addEventListener('touchmove', onDocumentTouchMove, false);
				
 $('#info').scroll(Scroll);
        
            }
        
            function loadTexture(path)
            {
        
                var material = new THREE.MeshBasicMaterial(
                {
                    map: new THREE.Texture(texture_placeholder)
                });
        
                var image = new Image();
        
                image.onload = function()
                {
        
                    this.loaded = true;
                    material.map.image = this;
                    render();
        
                };
        
                image.src = path;
        
                return material;
            }
        
        
        
        
        
            function onDocumentMouseDown(event)
            {
        
        
                event.preventDefault();
        
                isUserInteracting = true;
        
                onPointerDownPointerX = event.clientX;
                onPointerDownPointerY = event.clientY;
        
                onPointerDownLon = lon;
                onPointerDownLat = lat;
        
            }
        
            function onDocumentMouseMove(event)
            {
        
                if (isUserInteracting)
                {
        
                    lon = (onPointerDownPointerX - event.clientX) * 0.1 + onPointerDownLon;
                    lat = (event.clientY - onPointerDownPointerY) * 0.1 + onPointerDownLat;
        
                    render();
        
                }
            }
        
            function onDocumentMouseUp(event)
            {
        
                isUserInteracting = false;
                render();
        
            }
			var zxcTO;
			function  Scroll(){
			 clearTimeout(zxcTO);
			 zxcTO=setTimeout(function(){     
			    isUserInteracting = false;
                render();},100);
			}


        
            function onDocumentMouseWheel(event)
            {
        
                //    	fov -= event.wheelDeltaY * 0.05;
                //   	camera.projectionMatrix = THREE.Matrix4.makePerspective( fov, window.innerWidth / window.innerHeight, 1, 1100 );
                //	render();
            }
        
        
            function onDocumentTouchStart(event)
            {
        
                if (event.touches.length == 1)
                {
        
                    event.preventDefault();
        
                    onPointerDownPointerX = event.touches[0].pageX;
                    onPointerDownPointerY = event.touches[0].pageY;
        
                    onPointerDownLon = lon;
                    onPointerDownLat = lat;
                }
            }
        
            function onDocumentTouchMove(event)
            {
        
                if (event.touches.length == 1)
                {
        
                    event.preventDefault();
        
                    lon = (onPointerDownPointerX - event.touches[0].pageX) * 0.1 + onPointerDownLon;
                    lat = (event.touches[0].pageY - onPointerDownPointerY) * 0.1 + onPointerDownLat;
        
                    render();
        
                }
            }
        
            function render()
            {
        
                lat = Math.max(-85, Math.min(85, lat));
                phi = (90 - lat) * Math.PI / 180;
                theta = lon * Math.PI / 180;
        
                camera.target.position.x = 500 * Math.sin(phi) * Math.cos(theta);
                camera.target.position.y = 500 * Math.cos(phi);
                camera.target.position.z = 500 * Math.sin(phi) * Math.sin(theta);
        
                renderer.render(scene, camera);
        
            }