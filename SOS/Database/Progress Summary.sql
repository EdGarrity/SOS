SELECT TOP (1000) Detail.[ID]
      ,[Created_DTS]
	  ,Duration.AvgDuration
	  ,Duration.TotalDuration
	  ,Detail.[Run_Number]
      ,[Generation]
      ,[Generations_Completed_This_Session]
      ,[BestIndividual_Training_Error] 
      ,[BestIndividual_Training_Score]
      ,[BestIndividual_Test_Score]
      ,[BestIndividual_Training_Effort]
      ,[Average_Training_Error]
      ,[Standard_Deviation]
	  ,[Diversity]
	  ,[Diverse_Clusters]
      ,[Tempareture]
      ,[BestIndividual_Prev_Training_Error]
      ,[Stalled_Count]
      ,[Cool_Down_Count]
      ,[Include_Best_Individual_In_Breeding_Pool]
	  ,ABS([BestIndividual_Training_Error] - [BestIndividual_Prev_Training_Error]) AS Delta_Error
      ,[Number_Of_Training_Cases]
      ,[Number_Of_Test_Cases]
      ,[BestIndividual_ID]
      ,[Best_Genome]
      ,[Population_Size]
      ,[Alternation_Rate]
      ,[Uniform_Mutation_Rate]
      ,[Example_Case_Max_Length]
      ,[Example_Case_Upper_Range]
  FROM [SOS].[dbo].[ProgressLog] (NOLOCK) AS Detail
  INNER JOIN 
	(
		SELECT max_id as ID
		from
		(
			SELECT MAX([Generation]) as max_generation, MAX([ID]) as max_id, [Run_Number]
			  FROM [SOS].[dbo].[ProgressLog] (NOLOCK)
			  where [ID] > 2520
			  group by [Run_Number]
		) X
	) AS Last_Gen ON Detail.ID = Last_Gen.ID
  INNER JOIN
  (
	SELECT MAX([ID]) as ID, [Run_Number], 
	
	CONCAT(CONVERT(varchar, DATEDIFF(minute, max(max_Created_DTS), max([Created_DTS])) / 60), ':', CONVERT(varchar, DATEDIFF(minute, max(max_Created_DTS), max([Created_DTS])) % 60)) AS TotalDuration,
	
	CONCAT(CONVERT(varchar, DATEDIFF(minute, max(max_Created_DTS), max([Created_DTS])) / MAX([Generation]) / 60 ), ':', CONVERT(varchar, DATEDIFF(minute, max(max_Created_DTS), max([Created_DTS])) / MAX([Generation]) % 60)) AS AvgDuration

	FROM [SOS].[dbo].[ProgressLog] Y (NOLOCK),
	(
		SELECT MAX([Generation]) as max_generation, MAX([ID]) as max_id, [Run_Number] as max_run_number, max([Created_DTS]) as max_Created_DTS
		  FROM [SOS].[dbo].[ProgressLog] (NOLOCK)
		  where [ID] > 2520
		  group by [Run_Number]
	) X
	where [ID] > 2520 AND max_run_number = [Run_Number] - 1
	group by [Run_Number], x.max_id, x.max_run_number
  ) AS Duration ON Detail.ID = Duration.ID
ORDER BY Detail.ID DESC


