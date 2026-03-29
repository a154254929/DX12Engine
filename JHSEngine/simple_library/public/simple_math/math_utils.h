#pragma once
#include <vector>

#include "../simple_library_macro.h"
//����
#include "transformation/vector/vector_2d.h"
#include "transformation/vector/vector_3d.h"
#include "transformation/vector/vector_4d.h"

//����
#include "transformation/matrix/matrix_2x2.h"
#include "transformation/matrix/matrix_3x3.h"
#include "transformation/matrix/matrix_4x4.h"

//��Ԫ��
#include "transformation/quaternion/quat.h"

using namespace std;
namespace math_utils
{
    //����˷�
    fvector_3d SIMPLE_LIBRARY_API mul(const fvector_2d &in_3d, const fmatrix_3x3& in_matrix_3x3);
    fvector_3d SIMPLE_LIBRARY_API mul(const fvector_3d &in_3d, const fmatrix_3x3& in_matrix_3x3);
    fvector_4d SIMPLE_LIBRARY_API mul(const fvector_4d &in_3d, const fmatrix_4x4& in_matrix_3x3);

    float SIMPLE_LIBRARY_API angle_to_radian(float angle);
    float SIMPLE_LIBRARY_API radian_to_angle(float radian);

    //������ת
    void SIMPLE_LIBRARY_API rot_radian(float in_radian, fmatrix_3x3& in_world_matrix_3x3);
    void SIMPLE_LIBRARY_API rot_angle(float in_angle, fmatrix_3x3& in_world_matrix_3x3);

    //��������
    void SIMPLE_LIBRARY_API set_scale(const fvector_2d& in_scale, fmatrix_3x3& in_world_matrix_3x3);
    fmatrix_3x3 SIMPLE_LIBRARY_API set_scale(const fvector_2d& in_scale);

    //�ж�bool�ǲ�����һ����Χ��
    bool SIMPLE_LIBRARY_API scalar_neare_qual_float(float f1, float f2, float epsilon);
    bool SIMPLE_LIBRARY_API scalar_neare_qual_int(int i1, int i2, int epsilon);

    fmatrix_4x4 SIMPLE_LIBRARY_API matrix_perspective(float in_fov_radian, float aspect_ratio, float near_z, float far_z);

    //����Ŀ��
    fmatrix_4x4 SIMPLE_LIBRARY_API matrix_look_at_target(const fvector_4d &in_view_pos, const fvector_4d& in_target_pos, const fvector_4d& in_view_up);

    //����͸�Ӿ���
    fmatrix_4x4 SIMPLE_LIBRARY_API build_view_matrix(const fvector_4d& in_view_pos, const fmatrix_4x4& in_view_matrix);

    //��ת
    fmatrix_4x4 SIMPLE_LIBRARY_API matrix_rotation_y(const float angle);
    fmatrix_4x4 SIMPLE_LIBRARY_API matrix_rotation_x(const float angle);
    fmatrix_4x4 SIMPLE_LIBRARY_API matrix_rotation_z(const float angle);

    //��������ת
    fmatrix_4x4 SIMPLE_LIBRARY_API matrix_rotation_axis(const fvector_3d &axis,const float angle);

    //��Ԫ��ת����
    //����->����
    void SIMPLE_LIBRARY_API inertia_to_object(const fquat &in_quat, fmatrix_3x3& out_rotation_matrix);

    //����->����
    void SIMPLE_LIBRARY_API object_to_inertia(const fquat &in_quat, fmatrix_3x3& out_rotation_matrix);

    //����
    //����->���� 
    //Ĭ�ϵ� in_rotation_matrix �� ��->����
    fvector_3d SIMPLE_LIBRARY_API inertia_to_object(const fvector_3d& in_vector,const fmatrix_3x3& in_rotation_matrix);

    //����->����
    fvector_3d SIMPLE_LIBRARY_API object_to_inertia(const fvector_3d& in_vector, const fmatrix_3x3& in_rotation_matrix);

    //��ת����->��Ԫ��
    void SIMPLE_LIBRARY_API matrix_to_quat(const fmatrix_3x3& in_rotation_matrix, fquat& out_quat);

    //��ת����->��Ԫ��
    fquat SIMPLE_LIBRARY_API matrix_to_quat(const fmatrix_3x3& in_rotation_matrix);

    //��Ԫ������
    fquat SIMPLE_LIBRARY_API pow(const fquat &in_q,float in_exponent);
}