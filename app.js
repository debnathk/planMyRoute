let map;

function initMap() {
  map = new google.maps.Map(document.getElementById('map'), {
    center: {lat: -34.397, lng: 150.644},
    zoom: 8
  });
}

window.onload = initMap;

async function geocodeAddress(address) {
  const geocoder = new google.maps.Geocoder();
  return new Promise((resolve, reject) => {
    geocoder.geocode({'address': address}, (results, status) => {
      if (status === 'OK') {
        resolve(results[0].geometry.location);
      } else {
        reject('Geocode was not successful for the following reason: ' + status);
      }
    });
  });
}

async function getDistances(origins, destinations) {
  const service = new google.maps.DistanceMatrixService();
  return new Promise((resolve, reject) => {
    service.getDistanceMatrix({
      origins: origins,
      destinations: destinations,
      travelMode: 'DRIVING',
      unitSystem: google.maps.UnitSystem.METRIC
    }, (response, status) => {
      if (status !== 'OK') {
        reject('Error was: ' + status);
      } else {
        resolve(response);
      }
    });
  });
}

function solveTSP(distances) {
  // Implement TSP algorithm to find the shortest path
  // Placeholder for actual TSP solving logic
  const route = [...Array(distances.rows.length).keys()];
  return route;
}

async function plotRoute(locations) {
  const directionsService = new google.maps.DirectionsService();
  const directionsRenderer = new google.maps.DirectionsRenderer();
  directionsRenderer.setMap(map);

  const waypoints = locations.slice(1, locations.length - 1).map(location => ({
    location: location,
    stopover: true
  }));

  const request = {
    origin: locations[0],
    destination: locations[0],
    waypoints: waypoints,
    travelMode: 'DRIVING'
  };

  directionsService.route(request, (result, status) => {
    if (status === 'OK') {
      directionsRenderer.setDirections(result);
    } else {
      alert('Could not display directions due to: ' + status);
    }
  });
}

document.getElementById('calculateRoute').addEventListener('click', async () => {
  const originAddress = document.getElementById('origin').value;
  const locationAddresses = document.getElementById('locations').value.split('\n').map(addr => addr.trim()).filter(addr => addr !== '');

  try {
    const origin = await geocodeAddress(originAddress);
    const locations = await Promise.all(locationAddresses.map(addr => geocodeAddress(addr)));

    const allLocations = [origin, ...locations, origin];
    const distances = await getDistances(allLocations, allLocations);

    const route = solveTSP(distances);
    const orderedLocations = route.map(index => allLocations[index]);
    await plotRoute(orderedLocations);
  } catch (error) {
    console.error(error);
  }
});
