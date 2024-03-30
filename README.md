# planMyRoute: A tool for finding the best route for delivery

Delivery companies always use this type of tools for efficient delivery plans. Here, we have created a tool for efficiently planning the route for delivering automative batteries, where the source of delivery is our battery shop - Debnath Auto Centre, Kolkata. We have some dealers in Kolkata where we deliver automotive batteries in a frequent manner. The issue we are facing right now is that we don't have a proper delivery schedule. Batteries are currently delivered in a scattered manner, which causes much hassle for the delivery guys. Not to mention the excessive usage of fuel. A tool like this can be useful to overcome the aforementioned challenges. 

## What is TSP?
The travelling salesman problem, also known as the travelling salesperson problem (TSP), asks the following question: "Given a list of cities and the distances between each pair of cities, what is the shortest possible route that visits each city exactly once and returns to the origin city?" It is an NP-hard problem in combinatorial optimization, important in theoretical computer science and operations research.

## Strategies/tools used by different companies:
- ORION (UPS)
    - ORION (On-Road Integrated Optimization and Navigation) is a proprietary software by UPS for efficient route planning for multi-driver-multi-stop routes.
    - ORION efficiently solves the traveling salesman problem by determining the fastest and shortest routes.
    - Methods used by ORION:
        - Geospatial algorithms for complex metaheuristics.
        - Algorithms for vehicle routing and scheduling.
        - A routing engine that assists operators in hasslefree parking, loading-unoading and other issues.
- 


## References
- [How do couriers like FedEx plan their routes](https://www.routific.com/blog/how-do-experts-like-fedex-plan-delivery-routes#:~:text=Step%201%3A%20Sort%20routes%20by%20zip%2Fpostal%20code&text=By%20assigning%20each%20zone%20to,a%20shorter%20amount%20of%20time.)
- [ORION: How Route Optimization Keeps UPS Drivers On Time](https://www.roundtrip.ai/articles/ups-route-optimization-software)
