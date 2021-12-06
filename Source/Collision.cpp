#include "../Library/Misc.h"

#include "Collision.h"

// 円柱と円柱の交差判定
bool Collision::IntersectCylinderVsCylinder(JudgeCylinder& cylinder1, JudgeCylinder& cylinder2, Vec3& outCylinderPos2)
{
	if (cylinder1.position.y > cylinder2.position.y + cylinder2.height) return false;
	if (cylinder1.position.y + cylinder1.height < cylinder2.position.y) return false;

	DirectX::XMVECTOR p1 = Vec3(cylinder1.position.x, 0, cylinder1.position.z).GetXMVector();
	DirectX::XMVECTOR p2 = Vec3(cylinder2.position.x, 0, cylinder2.position.z).GetXMVector();
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(p2, p1);
	float length = Math::Vector3Length(Vec);

	float range = cylinder1.radius + cylinder2.radius;
	if (length > range) return false;

	Vec = DirectX::XMVector3Normalize(Vec);

	float diff = range - length;
	Vec = DirectX::XMVectorScale(Vec, diff);

	DirectX::XMVECTOR Velocity = DirectX::XMVectorScale(Vec, 1.0f);
	p2 = DirectX::XMVectorAdd(p2, Velocity);

	DirectX::XMStoreFloat3(&outCylinderPos2, p2);

	return true;
}

// 球と円柱の交差判定
bool Collision::InversectSphereVsSylinder(JudgeSphere& sphere, JudgeCylinder& cylinder, Vec3& outCylinderPosition)
{
	// 高さチェック
	if (sphere.position.y + sphere.radius < cylinder.position.y) return false;
	if (sphere.position.y - sphere.radius > cylinder.position.y + cylinder.height) return false;

	// XZ平面での範囲チェック
	float vx = cylinder.position.x - sphere.position.x;
	float vz = cylinder.position.z - sphere.position.z;
	float range = sphere.radius + cylinder.radius;
	float distXZ = sqrtf(vx * vx + vz * vz);
	if (distXZ > range) return false;

	// 球が円柱を押し出す
	vx += distXZ;
	vz += distXZ;
	outCylinderPosition.x = sphere.position.x + vx * range;
	outCylinderPosition.y = cylinder.position.y;
	outCylinderPosition.z = sphere.position.z + vz * range;

	return true;
}

// レイとモデルの交差判定
bool Collision::IntersectRayVsModel(const Vec3& start, const Vec3& end, const Model* model, HitResult& result)
{
	DirectX::XMVECTOR WorldStart = start.GetXMVector();
	DirectX::XMVECTOR WorldEnd = end.GetXMVector();
	DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
	DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);

	result.distance = Math::Vector3Length(WorldRayLength);

	bool hit = false;
	const std::shared_ptr<ModelResource> resource = model->GetResource();
	for (const ModelResource::Mesh& mesh : resource->GetMeshes())
	{
		const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);

		DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);
		DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

		DirectX::XMVECTOR Start = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
		DirectX::XMVECTOR End = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
		DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
		DirectX::XMVECTOR Dir = DirectX::XMVector3Normalize(Vec);
		DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);

		float neart = Math::Vector3Length(Length);

		const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
		const std::vector<u_int> indices = mesh.indices;

		int materialIndex = -1;
		DirectX::XMVECTOR HitPosition = {};
		DirectX::XMVECTOR HitNormal = {};
		for (const ModelResource::Subset& subset : mesh.subsets)
		{
			for (u_int i = 0; i < subset.indexCount; i += 3)
			{
				u_int index = subset.startIndex + i;

				const ModelResource::Vertex& a = vertices.at(indices.at(index));
				const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
				const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));

				DirectX::XMVECTOR A = a.position.GetXMVector();
				DirectX::XMVECTOR B = b.position.GetXMVector();
				DirectX::XMVECTOR C = c.position.GetXMVector();

				DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
				DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
				DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

				DirectX::XMVECTOR Normal = DirectX::XMVector3Cross(AB, BC);

				DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Dir, Normal);
				float dot = 0.0f;
				DirectX::XMStoreFloat(&dot, Dot);
				if (dot >= 0.0f) continue;

				DirectX::XMVECTOR V = DirectX::XMVectorSubtract(A, Start);
				DirectX::XMVECTOR T = DirectX::XMVectorDivide(DirectX::XMVector3Dot(Normal, V), Dot);
				float t = 0.0f;
				DirectX::XMStoreFloat(&t, T);
				if (t < 0.0f || t > neart) continue;

				DirectX::XMVECTOR Position = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(Dir, T), Start);

				dot = Utility::IntersectionInsideLine(Position, A, AB, Normal);
				if (dot < 0.0f) continue;

				dot = Utility::IntersectionInsideLine(Position, B, BC, Normal);
				if (dot < 0.0f) continue;

				dot = Utility::IntersectionInsideLine(Position, C, CA, Normal);
				if (dot < 0.0f) continue;

				neart = t;

				HitPosition = Position;
				HitNormal = Normal;
				materialIndex = subset.materialIndex;
			}
		}
		if (materialIndex > -1)
		{
			DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);
			DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
			float distance = Math::Vector3Length(WorldCrossVec);

			if (result.distance > distance)
			{
				DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal, WorldTransform);

				result.distance = distance;
				result.materialIndex = materialIndex;
				DirectX::XMStoreFloat3(&result.position, WorldPosition);
				DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldNormal));
				hit = true;
			}
		}
	}

	return hit;
}
