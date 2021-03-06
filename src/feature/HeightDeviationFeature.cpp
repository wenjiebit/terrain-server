#include <terrain_server/feature/HeightDeviationFeature.h>


namespace terrain_server
{

namespace feature
{

HeightDeviationFeature::HeightDeviationFeature(double flat_height_deviation,
											   double max_height_deviation,
											   double min_allowed_height) :
													   flat_height_deviation_(flat_height_deviation),
													   max_height_deviation_(max_height_deviation),
													   min_allowed_height_(min_allowed_height)
{
	name_ = "Height Deviation";
}


HeightDeviationFeature::~HeightDeviationFeature()
{

}


void HeightDeviationFeature::computeCost(double& cost_value,
										 const dwl::Terrain& terrain_info)
{
	// Setting the grid resolution of the gridmap
	space_discretization_.setEnvironmentResolution(terrain_info.resolution, true);
	space_discretization_.setStateResolution(terrain_info.resolution);

	// Getting the cell position
	Eigen::Vector2d cell_position = terrain_info.position.head(2);
	dwl::Vertex cell_vertex;
	space_discretization_.stateToVertex(cell_vertex, cell_position);
	space_discretization_.vertexToState(cell_position, cell_vertex);

	// Putting minimum cost to voxel with low height
	if (terrain_info.height_map->find(cell_vertex)->second < min_allowed_height_) {
		if (terrain_info.height_map->count(cell_vertex) > 0) {
			cost_value = max_cost_;
			return;
		}
	}

	// Computing the average height of the neighboring area
	double height_average = 0, height_deviation = 0, estimated_height_deviation = 0;
	int counter = 0, estimated_counter = 0;

	Eigen::Vector2d boundary_min, boundary_max;
	boundary_min(0) = neightboring_area_.min_x + cell_position(0);
	boundary_min(1) = neightboring_area_.min_y + cell_position(1);
	boundary_max(0) = neightboring_area_.max_x + cell_position(0);
	boundary_max(1) = neightboring_area_.max_y + cell_position(1);
	for (double y = boundary_min(1); y <= boundary_max(1); y += neightboring_area_.resolution) {
		for (double x = boundary_min(0); x <= boundary_max(0); x += neightboring_area_.resolution) {
			Eigen::Vector2d coord;
			coord(0) = x;
			coord(1) = y;
			dwl::Vertex vertex_2d;
			space_discretization_.coordToVertex(vertex_2d, coord);

			if (terrain_info.height_map->count(vertex_2d) > 0) {
				double height = terrain_info.height_map->find(vertex_2d)->second;
				height_average += height;
				counter++;
			}
		}
	}
	if (counter != 0) {
		height_average /= counter;

		// Computing the standard deviation of the height
		for (double y = boundary_min(1); y <= boundary_max(1); y += neightboring_area_.resolution) {
			for (double x = boundary_min(0); x <= boundary_max(0); x += neightboring_area_.resolution) {
				Eigen::Vector2d coord;
				coord(0) = x;
				coord(1) = y;
				dwl::Vertex vertex_2d;
				space_discretization_.coordToVertex(vertex_2d, coord);

				if (terrain_info.height_map->count(vertex_2d) > 0) {
					height_deviation += fabs(terrain_info.height_map->find(vertex_2d)->second - height_average);
				} else {
					// Computing the estimated ground
					Eigen::Vector2d height_boundary_min, height_boundary_max;
					height_boundary_min(0) = neightboring_area_.min_x + coord(0);
					height_boundary_min(1) = neightboring_area_.min_y + coord(1);
					height_boundary_max(0) = neightboring_area_.max_x + coord(0);
					height_boundary_max(1) = neightboring_area_.max_y + coord(1);
					double estimated_height = 0;
					int height_counter = 0;
					double resolution = neightboring_area_.resolution;
					for (double y_e = height_boundary_min(1); y_e < height_boundary_max(1); y_e += resolution) {
						for (double x_e = height_boundary_min(0); x_e < height_boundary_max(0); x_e += resolution) {
							Eigen::Vector2d height_coord;
							height_coord(0) = x_e;
							height_coord(1) = y_e;
							dwl::Vertex height_vertex_2d;
							space_discretization_.coordToVertex(height_vertex_2d, height_coord);

							if (terrain_info.height_map->count(height_vertex_2d) > 0)
								estimated_height += terrain_info.height_map->find(height_vertex_2d)->second;
							else
								estimated_height += terrain_info.min_height;

							height_counter++;
						}
					}

					if (height_counter != 0) {
						estimated_height /= height_counter;
						estimated_height_deviation += fabs(estimated_height - height_average);
						estimated_counter++;
					}
				}
			}
		}

		height_deviation /= counter;

		if (estimated_counter != 0)
			estimated_height_deviation /= estimated_counter;

		double total_heigh_deviation = height_deviation + estimated_height_deviation;

		if (total_heigh_deviation <= flat_height_deviation_)
			cost_value = 0.;
		else if (total_heigh_deviation < max_height_deviation_) {
			cost_value = -log(1 - (total_heigh_deviation - flat_height_deviation_) /
					(max_height_deviation_ - flat_height_deviation_));
			if (max_cost_ < cost_value)
				cost_value = max_cost_;
		} else
			cost_value = max_cost_;
	} else
		cost_value = 0.;
}

} //@namespace feature
} //@namespace terrain_server
